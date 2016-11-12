import ocamlDocCss from './css/ocamlDoc.css';
import ocamlLogo from './images/ocamlLogo128.png';
import reasonLogo from './images/logo128.png';
import hljs from 'highlight.js/lib/highlight.js';
import addToggles from './addToggles';
import getLangList from './getLangList';

hljs.registerLanguage('ocaml', require('highlight.js/lib/languages/ocaml'));
hljs.configure({classPrefix: '', languages: ['ocaml']});

const ocamlDocStyleTag = document.createElement('link');

ocamlDocStyleTag.type = 'text/css';
ocamlDocStyleTag.rel = 'stylesheet';
ocamlDocStyleTag.href = chrome.extension.getURL(ocamlDocCss);

let styleTags = [ocamlDocStyleTag];
let body = document.body.outerHTML;
let syntaxSwap;

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
    let maybeTextSibilng;
    let usesTypeTable = false;

    const maybeTypeTable = pre.nextSibling;
    let text = pre.innerText;
    if (maybeTypeTable && maybeTypeTable.classList && maybeTypeTable.classList.contains('typetable')) {
      // who came up with this markup??
      usesTypeTable = true;
      text += maybeTypeTable.innerText;
      if (maybeTypeTable.nextSibling && maybeTypeTable.nextSibling.nodeType === Node.TEXT_NODE) {
        maybeTextSibilng = maybeTypeTable.nextSibling;
        text += maybeTextSibilng.nodeValue;
      }
    }
    const relevantEls = usesTypeTable ? [pre, maybeTypeTable] : [pre];
    const hrefs = getNormalizedLinks(relevantEls);
    const ids = getNormalizedIds(relevantEls);
    let outLanguage;
    chrome.runtime.sendMessage({in: normalizeText(text)},
      ({out: [conversionType, out]}) => {
        if (conversionType !== 'Failure') {
          const [inLang, outLang] = getLangList(conversionType);
          outLanguage = outLang;
          if (usesTypeTable) {
            removeEl(maybeTypeTable);
            maybeTextSibilng && removeEl(maybeTextSibilng);
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
        if (finished >= total) {
          syntaxSwap.style.backgroundImage = outLanguage === 'ML' || outLanguage === 'MLI'
            ? `url("${chrome.extension.getURL(ocamlLogo)}")`
            : `url("${chrome.extension.getURL(reasonLogo)}")`

          if (type === 'initial' && window.location.hash) {
            window.location.href = window.location.href
          }
        }
      }
    )
  }
}

function removeEl(el) {
  el.parentNode.removeChild(el);
}

export default function() {
  swapStyleSheets();
  syntaxSwap = addToggles(swapSyntax, swapStyleSheets);
  swapSyntax('initial');
}
