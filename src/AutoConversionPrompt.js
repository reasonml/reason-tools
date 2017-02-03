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

function swapSyntax(type) {
  let pres = document.getElementsByTagName('pre');
  let usesFakePres = false;
  if (!pres.length) {
    pres = document.getElementsByClassName('def'); // why is this so hard?
    usesFakePres = true;
  }
  const total = pres.length;
  let finished = 0;
  for (var p of pres) {
    const pre = p;
    let maybeTextSibling;
    let usesTypeTable = false;

    const maybeTypeTable = pre.nextSibling;
    let text = pre.innerText;
    if (maybeTypeTable && maybeTypeTable.classList && maybeTypeTable.classList.contains('typetable')) {
      // who came up with this markup??
      usesTypeTable = true;
      text += maybeTypeTable.innerText;
      if (maybeTypeTable.nextSibling && maybeTypeTable.nextSibling.nodeType === Node.TEXT_NODE) {
        maybeTextSibling = maybeTypeTable.nextSibling;
        text += maybeTextSibling.nodeValue;
      }
    }
    const relevantEls = usesTypeTable ? [pre, maybeTypeTable] : [pre];
    const hrefs = getNormalizedLinks(relevantEls);
    const ids = getNormalizedIds(relevantEls);
    chrome.runtime.sendMessage({in: normalizeText(text)},
      ({out: [conversionType, out]}) => {
        if (conversionType !== 'Failure') {
          if (usesTypeTable) {
            removeEl(maybeTypeTable);
            maybeTextSibling && removeEl(maybeTextSibling);
          }
          Object.keys(hrefs).forEach((text) => {
            out = out.replace(
              new RegExp(`\\b${escapeRegExp(text)}\\b`, 'g'),
              `<a href=${hrefs[text]}>${text}</a>`,
            );
          });
          Object.keys(ids).forEach((text) => {
            out = out.replace(
              new RegExp(`\\b${escapeRegExp(text)}\\b`),
              `<span class="reason_tools_anchor" id=${ids[text]}>${text}</span>`,
            );
          });
          pre.innerHTML = usesFakePres
            ? `<pre>${out}</pre>`
            : out;
          hljs.highlightBlock(pre);
        }
        finished++;
        if (type === 'initial' && finished >= total && window.location.hash) {
          window.location.href = window.location.href
        }
      }
    )
  }
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
