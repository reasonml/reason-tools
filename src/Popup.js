var Refmt = require('../_build/refmt/src/app.js');

document.addEventListener('DOMContentLoaded', function() {
  var inTextArea = document.getElementById('in');
  var outPre = document.getElementById('out');

  inTextArea.addEventListener('input', function() {
    var outText = Refmt.refmt(inTextArea.value);
    outPre.textContent = outText;
  });
})
