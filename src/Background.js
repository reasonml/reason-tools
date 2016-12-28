import Background from '../_build/background/src/app.js';

chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
  const response = {};
  request.type && request.type.forEach((type) => {
    if(Background[type]) {
      response[type] = Background[type](request.payload);
    }
  });
  Background.omnitop(request.payload, (str) => console.log(str));
  sendResponse(response);
});
