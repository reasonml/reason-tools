import reasonLogo from './images/logo128.png';

const syntaxSwap = document.createElement('div');
const cssSwap = document.createElement('div');

function insertEl(el) {
  document.body.appendChild(el);
}

export default function(swapSyntax, swapStyleSheets) {
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
  if (swapStyleSheets) {
    cssSwap.style.top = '90px';
    cssSwap.innerText = '</>';
    cssSwap.className = 'reason_tools_button';
    cssSwap.onclick = swapStyleSheets;
    insertEl(cssSwap);
  }

  if (swapSyntax) {
    syntaxSwap.style.top = '40px';
    syntaxSwap.className = 'reason_tools_button';
    syntaxSwap.onclick = swapSyntax;
    syntaxSwap.style.backgroundImage = `url("${chrome.extension.getURL(reasonLogo)}")`
    syntaxSwap.style.backgroundSize = 'cover';
    insertEl(syntaxSwap);    
  }

  return syntaxSwap;
}
