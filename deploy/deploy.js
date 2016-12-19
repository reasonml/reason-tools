const fs = require('fs');
const path = require('path');
const ChromeWebstoreApi = require('chrome-store-api').Webstore;
const ChromeTokenManager = require('chrome-store-api').TokenManager;
const ChromeFileStorage = require('chrome-store-api').FileStorage;

const reasonToolsZip = path.join(
  __dirname,
  '../_build/extension/reason-tools.zip'
);

function deployChrome(toTrustedTesters) {
  const chromeCode = process.env.CHROME_CODE;
  const chromeClientId = process.env.CHROME_CLIENT_ID;
  const chromeClientSecret = process.env.CHROME_CLIENT_SECRET;

  const storage = new ChromeFileStorage(path.join(
    process.env.CIRCLE_ARTIFACTS,
    'chromeTokenStorage.json'
  ));
  const tokenManager = new ChromeTokenManager(
    chromeCode,
    chromeClientId,
    chromeClientSecret,
    storage
  );
  const api = new ChromeWebstoreApi(tokenManager);

  api
    .update('kmdelnjbembbiodplmhgfjpecibfhadd', fs.readFileSync(reasonToolsZip))
    .then((data) => api.publish(data.id))
    .then((data) => console.log(data))
    .catch((err) => console.log(err));
};

deployChrome();
