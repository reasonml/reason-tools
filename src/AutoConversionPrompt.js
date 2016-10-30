import ocamlDocCss from './css/ocamlDoc.css';

const ocamlDocStyleTag = document.createElement('link');
ocamlDocStyleTag.type = 'text/css';
ocamlDocStyleTag.rel = 'stylesheet';
ocamlDocStyleTag.href = chrome.extension.getURL('ocamlDoc.css');

let styleTags = [ocamlDocStyleTag];

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
  'Chapter',
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
    'val',
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
      sheet.parentNode.removeChild(sheet);
    }
  }

  for(let tag of styleTags) {
    document.head.appendChild(tag);
  }

  styleTags = tempSheets;
}

function swapSyntax() {
  let pres = document.getElementsByTagName('pre');
  let usesFakePres = false;
  if (!pres.length) {
    pres = document.getElementsByClassName('def'); // why is this so hard?
    usesFakePres = true;
  }
  for (var pre of pres) {
    const forEverPre = pre;
    const inText = pre.innerText;
    chrome.runtime.sendMessage({in: inText},
      ({out: [conversionType, out]}) => {
        if (conversionType !== 'Failure') {
          forEverPre.innerHTML = usesFakePres
            ? `<pre>${out}</pre>`
            : out;
        }
      }
    )
  }
}

if (mightBeOcamlDoc()) {
  swapStyleSheets();
  swapSyntax();
}
