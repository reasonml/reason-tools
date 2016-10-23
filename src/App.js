var Refmt = require('../_build/refmt/src/app.js');

function isCodeBlock(node) {
  var name = node.tagName.toUpperCase();
  return name === "PRE" || name === "CODE";
}

document.addEventListener('DOMContentLoaded', function() {
  var observer = new MutationObserver(function(mutations) {
    mutations.forEach(function(mutation) {
      mutation.addedNodes.forEach(function(node) {
        if (isCodeBlock(node)) {
          console.log(Refmt.refmt(node.innerText))
        }
      });
    });
  });

  observer.observe(document.body, {
    attributes: false,
    childList: true,
    characterData: true,
  });
});
