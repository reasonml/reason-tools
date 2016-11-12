import ocamlDocCss from './css/ocamlDoc.css';
import ocamlLogo from './images/ocamlLogo128.png';
import reasonLogo from './images/logo128.png';
import hljs from 'highlight.js/lib/highlight.js';
import addToggles from './addToggles';

let syntaxSwap;


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
  const total = pres.length;
  let finished = 0;
  for (var p of pres) {
    const pre = p;
    let text = pre.innerText;
    const relevantEls = [pre];
    const hrefs = getNormalizedLinks(relevantEls);
    const ids = getNormalizedIds(relevantEls);
    chrome.runtime.sendMessage({in: normalizeText(text)},
      ({out: [conversionType, out]}) => {
        if (conversionType !== 'Failure') {
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
          pre.innerHTML = out;
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


export default function() {
  syntaxSwap = addToggles(swapSyntax);
  console.log('converting rwo');
  swapSyntax('initial');
}
