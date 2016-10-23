var Refmt = require('../_build/refmt/src/app.js');

var trackedElements = [];
function isCodeBlock(node) {
  var name = node.tagName.toUpperCase();
  var isPre = name === 'PRE';
  if (isPre) {
    var isTracked = trackedElements.indexOf(node) > -1;
    if (!isTracked) {
      trackedElements.push(node);
      return true;
    }
  }
  return false;
}

function tryRefmt(node) {
  if (isCodeBlock(node)) {
    return Refmt.refmt(node.innerText);
  }
  return null;
}

function removeNode(node) {
  node.parentNode.removeChild(node);
}

function refmtPotentialNode(node) {
  var refmtText = tryRefmt(node);
  if (!refmtText) {
    return;
  }
  var tooltip;
  node.addEventListener('mouseenter', function(event) {
    if (tooltip) {
      removeNode(tooltip);
    }
    var target = event.target.getBoundingClientRect();
    var text = document.createTextNode(refmtText);
    tooltip = document.createElement('pre');
    tooltip.appendChild(text);
    tooltip.style.position = 'fixed';
    tooltip.style.top = '0px';
    tooltip.style.right = '0px';
    tooltip.style.backgroundColor = 'white';
    tooltip.style.padding = '10px';

    document.body.appendChild(tooltip);
  });

  node.addEventListener('mouseleave', function() {
    if (tooltip) {
      removeNode(tooltip);
      tooltip = null;
    }
  })
}

var pres = document.getElementsByTagName('PRE');

var observer = new MutationObserver(function(mutations) {
  mutations.forEach(function(mutation) {
    mutation.addedNodes.forEach(refmtPotentialNode);
  });
  for (var i = 0; i < pres.length; i++) {
    var pre = pres.item(i);
    refmtPotentialNode(pre);
  }
});

observer.observe(document.body, {
  attributes: false,
  childList: true,
  characterData: true,
  subtree: true,
});

for (var i = 0; i < pres.length; i++) {
  var pre = pres.item(i);
  refmtPotentialNode(pre);
}
