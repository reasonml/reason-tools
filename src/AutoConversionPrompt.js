import ocamlDocCss from './css/ocamlDoc.css';
import ocamlLogo from './images/ocamlLogo128.png';
import reasonLogo from './images/logo128.png';
import hljs from 'highlight.js/lib/highlight.js';

hljs.registerLanguage('ocaml', require('highlight.js/lib/languages/ocaml'));
hljs.configure({classPrefix: '', languages: ['ocaml']});

const ocamlDocStyleTag = document.createElement('link');
const syntaxSwap = document.createElement('div');
const cssSwap = document.createElement('div');

ocamlDocStyleTag.type = 'text/css';
ocamlDocStyleTag.rel = 'stylesheet';
ocamlDocStyleTag.href = chrome.extension.getURL(ocamlDocCss);

let styleTags = [ocamlDocStyleTag];
let body = document.body.outerHTML;

function hasClassName(className) {
  const els = document.getElementsByClassName(className);
  return els.length > 0;
}

const ocamlishRels = [
  'Start',
  'previous',
  'next',
  'Up',
  'Appendix',
  'Section',
  'Subsection',
];

function hasOcamlRels() {
  return Array.prototype.map.call(
    document.getElementsByTagName('link'),
    (link) => ocamlishRels.indexOf(link.getAttribute('rel')) > -1,
  ).filter(Boolean).length >= 3;
}

function hasCommonClassNames() {
  return [
    'keyword',
    'type',
    'deprecated',
    'mod',
    'modt',
    'typ',
    'spec',
    'def',
    'ext',
    'exn',
    'cls',
    'clst',
    'include',
    'cons',
    'paramstable',
    'sig_block',
  ].map(hasClassName).filter(Boolean).length >= 3;
}

function hasUniqueClassNames() {
  return [
    'odoc-doc',
    'package-index',
  ].map(hasClassName).filter(Boolean).length > 0;
}

function mightBeOcamlDoc() {
  return hasUniqueClassNames() || hasOcamlRels() || hasCommonClassNames();
}

function swapStyleSheets() {
  const stylesheets = document.getElementsByTagName('link');
  const tempSheets = [];

  for(let sheet of stylesheets) {
    if(sheet.getAttribute('rel') === 'stylesheet') {
      tempSheets.push(sheet);
      removeEl(sheet);
    }
  }

  for(let tag of styleTags) {
    document.head.appendChild(tag);
  }

  document.body.outerHTML = body;

  styleTags = tempSheets;
}

function normalizeText(text) {
  return text.trim().replace(/[^\x00-\x7F]/g, ' ').replace(String.fromCharCode(65533), '');
}

function getNormalizedLinks(els) {
  let anchors = els.reduce(
    (anchors, el) => [...anchors, ...el.getElementsByTagName('a')],
    [],
  );
  const textToHref = {};
  for (var a of anchors) {
    textToHref[normalizeText(a.innerText)] = a.href;
  }
  return textToHref;
}

function getNormalizedIds(els) {
  let spans = els.reduce(
    (spans, el) => [...spans, ...el.querySelectorAll('[id]')],
    [],
  );
  const textToId = {};
  for (var span of spans) {
    const words = normalizeText(span.innerText).split(' ')
    const lastWord = words[words.length - 1];
    textToId[lastWord] = span.id;
  }
  return textToId;
}

function escapeRegExp(str) {
  return str.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, "\\$&");
}

function getTypeTable(pre) {
  const removables = [];
  const el = pre.nextSibling;
  if (el && el.classList && el.classList.contains('typetable')) {
      removables.push(el);
      let text = el.innerText;

      if (el.nextSibling && el.nextSibling.nodeType === Node.TEXT_NODE) {
        removables.push(el.nextSibling);
        text += el.nextSibling.nodeValue;
      }

      return { el, text, remove: () => removables.forEach(removeEl) };
  } else {
    return null;
  }
}

function getPreTargets() {
  const els = document.getElementsByTagName('pre');

  return [...els]
    .map(el => {
      let text = el.innerText;
      const maybeTypeTable = getTypeTable(el);

      if (maybeTypeTable) {
        text += maybeTypeTable.text;
      }

      return {
        els: maybeTypeTable ? [el, maybeTypeTable.el] : [el],
        text,
        replace: html => {
            el.innerHTML = html;

            if (maybeTypeTable) {
              maybeTypeTable.reomve();
            }

            return el;
        }
      };
    });
}

function getDefTargets() {
  const els = document.getElementsByClassName('def');

  return [...els].map(el => ({
    els: [el],
    text: el.innerText,
    replace: html => {
      el.innerhtml = `<pre>${html}</pre>`;
      return el;
    }
  }));
}

function getLstListingTargets() {
  const els = document.getElementsByClassName('lstlisting');
  return [...els].map(el => ({
    els: [el],
    text: el.innerText,
    replace: html => {
        const parent = el.parentNode;
        parent.innerHTML = `<pre>${html}</pre>`;
        return parent;
    }
  }));
}

function getCodeTargets() {
  const els = document.getElementsByTagName('code');
  return [...els].map(el => ({
    els: [el],
    text: el.innerText,
    replace: html => {
      el.innerHTML = html;
      return el;
    }
  }));
}

function getTargets() {
    return [
      ...getPreTargets(),
      ...getDefTargets(),
      ...getLstListingTargets(),
      ...getCodeTargets()
    ];
}

function replaceHrefs(hrefs, out) {
  return Object.keys(hrefs).reduce((out, text) =>
    out.replace(
      new RegExp(`\\b${escapeRegExp(text)}\\b`, 'g'),
      `<a href=${hrefs[text]}>${text}</a>`,
    ), out);
}

function replaceIds(ids, out) {
  return Object.keys(ids).reduce((out, text) =>
    out.replace(
      new RegExp(`\\b${escapeRegExp(text)}\\b`),
      `<span class="reason_tools_anchor" id=${ids[text]}>${text}</span>`,
    ), out);
}

function readjustViewport() {
  if (window.location.hash) {
    window.location.href = window.location.href
  }
}

function swapSyntax(type) {
  const targets = getTargets();
  let finished = 0;

  targets.forEach(({ els, text, replace }) => {
    const hrefs = getNormalizedLinks(els);
    const ids = getNormalizedIds(els);

    chrome.runtime.sendMessage(
      { in: normalizeText(text) },
      (msg) => {
        if (!msg) { return; } // msg might, for unknown reasons, sometimes be undefined

        let { out: [conversionType, out] } = msg;

        if (conversionType !== 'Failure') {
          out = replaceHrefs(hrefs, out);
          out = replaceIds(ids, out);

          const el = replace(out);
          hljs.highlightBlock(el);
        }

        // we're in an async callback, so keep track of when we're finished by keeping count
        finished++;
        if (type === 'initial' && finished >= targets.length) {
          // when we're done, the DOM has most likely been shifted,
          // so we need to go back to where we're supposed to be
          readjustViewport();
        }
      }
    )
  });

  syntaxSwap.style.backgroundImage =
    syntaxSwap.style.backgroundImage === `url("${chrome.extension.getURL(reasonLogo)}")`
      ? `url("${chrome.extension.getURL(ocamlLogo)}")`
      : `url("${chrome.extension.getURL(reasonLogo)}")`
}

function insertEl(el) {
  document.body.appendChild(el);
}

function removeEl(el) {
  el.parentNode.removeChild(el);
}

function addSwappers() {
  const styleTag = document.createElement('style');
  styleTag.type = 'text/css';
  styleTag.innerText = `
  .reason_tools_button.reason_tools_button.reason_tools_button {
    position: fixed;
    right: 0;
    height: 50px;
    width: 50px;
    background-color: black;
    color: white;
    font-family: monospace;
    display: flex;
    justify-content: center;
    align-items: center;
    font-weight: 900;
    opacity: 0.6;
  }
  .reason_tools_button.reason_tools_button.reason_tools_button:hover {
    opacity: 1;
    cursor: pointer;
  }
  .reason_tools_anchor {
    color: #cec47f;
  }
  .reason_tools_anchor:before {
    content: '';
    float: left;
    position: relative;
    width: 0;
    height: 50px;
    margin-top: -50px;
  }
  .reason_tools_anchor:target:after {
    content: '';
    position: relative;
    width: 4px;
    margin-left: -4px;
    height: 18px;
    float: left;
    background-color: #97B98c;
    left: -10px;
  }
  `;
  insertEl(styleTag);
  cssSwap.style.top = '90px';
  cssSwap.innerText = '</>';
  cssSwap.className = 'reason_tools_button';
  cssSwap.onclick = swapStyleSheets;
  insertEl(cssSwap);

  syntaxSwap.style.top = '40px';
  syntaxSwap.className = 'reason_tools_button';
  syntaxSwap.onclick = swapSyntax;
  syntaxSwap.style.backgroundImage = `url("${chrome.extension.getURL(reasonLogo)}")`
  syntaxSwap.style.backgroundSize = 'cover';
  insertEl(syntaxSwap);
}

if (mightBeOcamlDoc()) {
  swapStyleSheets();
  swapSyntax('initial');
  addSwappers();
}
