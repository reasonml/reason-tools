require('./popup.html');
require('./images/logo19.png');
require('./images/logo38.png');
require('./images/logo128.png');
require('./css/codemirror.css');
require('./css/oceanic-next.css');

import CopyToClipboard from 'react-copy-to-clipboard';
import React from 'react';
import ReactDOM from 'react-dom';
import CodeMirror from 'react-codemirror';
import 'codemirror/mode/javascript/javascript';
import 'codemirror/mode/mllike/mllike';

import {
  popup,
  popupColumn,
  popupInNOut,
  popupContext,
  savedBadge,
  contextLink,
  contextTitle,
} from './styles';

const getLangList = (conversionType) => {
  return conversionType && conversionType !== 'Failure'
    ? conversionType.split('to')
    : [null, null];
}

class Popup extends React.Component {
  state = {copy: false, in: '', out: '', inLang: null, outLang: null};

  constructor(props) {
    super(props);
    const normalizedHash = atob(window.location.hash.slice(1));
    this._initialText = props.initialSelectedText || normalizedHash;
  }

  componentDidMount() {
    if (this._initialText) {
      this._refmt(this._initialText);
    }
    if (this._in) {
      this._in.focus();
      this._in.getCodeMirror().execCommand('selectAll')
    }
  }

  render() {
    // extra cautious here because of the async on _refmt
    // sometimes typing can flash the wrong lang very quickly
    const hasConverted = this.state.in.trim() && this.state.out.trim();

    return (
      <div style={popup}>
        <div style={popupColumn}>
          <h1 style={popupContext}>
            In{
              hasConverted && this.state.inLang
                ? ` (${this.state.inLang})`
                : null}
          </h1>
          <CodeMirror
            style={popupInNOut}
            ref={(ref) => this._in = ref}
            defaultValue={this._initialText}
            value={this.state.in}
            onChange={this._refmt}
            options={{
              mode: this.state.inLang === 'ML' ? 'text/x-ocaml' : 'javascript',
              theme: 'oceanic-next',
            }}
          />
        </div>
        <div style={popupColumn}>
          <h1 style={popupContext}>
            <span style={contextTitle}>Out{
              hasConverted && this.state.outLang
                ? ` (${this.state.outLang})`
                : null}
              </span>
            <CopyToClipboard text={this.state.out} onCopy={this._handleCopy}>
              <a style={contextLink} href="#">copy</a>
            </CopyToClipboard>
            <a style={contextLink} href="#" onClick={this._handleOpen}>
              <svg
                height="3vh"
                width="3vh"
                viewBox="0 0 748 1024"
                xmlns="http://www.w3.org/2000/svg">
                <path
                  d="M640 768H128V257.90599999999995L256 256V128H0v768h768V576H640V768zM384 128l128 128L320 448l128 128 192-192 128 128V128H384z"
                  fill={contextLink.color}
                />
              </svg>
            </a>
          </h1>
          <CodeMirror
            style={popupInNOut}
            value={this.state.out}
            options={{
              mode: this.state.outLang === 'ML' ? 'text/x-ocaml' : 'javascript',
              theme: 'oceanic-next',
              readOnly: true,
            }}
          />
          {this.state.copy && <div style={savedBadge}>{'\u2713'}</div>}
        </div>
      </div>
    );
  }

  _handleCopy = () => {
    this.setState({copy: true});
    setTimeout(() => this.setState({copy: false}), 2500);
  }

  _handleOpen = () => {
    const inData = btoa(this.state.in);
    chrome.tabs.create({url: 'popup.html#' + inData});
  }

  _refmt = (value) => {
    this.setState({in: value});
    // this isn't guaranteed to be sync or speedy, so
    // don't set this.state.in here, since it could cause lag.
    chrome.runtime.sendMessage({in: value},
      ({out: [conversionType, out, ...rest]}) => {
        const [inLang, outLang] = getLangList(conversionType);
        this.setState({out, inLang, outLang});
      }
    );

    chrome.storage.local.set({latestRefmtString: value});
  }
}

document.addEventListener('DOMContentLoaded', () => {
    Promise.all([
      new Promise((resolve) =>
        chrome.tabs.executeScript(
          {code: 'window.getSelection().toString();'},
          resolve,
        )
      ),
      new Promise((resolve) =>
        chrome.storage.local.get(
          'latestRefmtString',
          ({latestRefmtString: selection}) => resolve(selection),
        )
      ),
    ]).then(([selection, latestRefmtString]) => {
      // in ff you get a single result, chrome returns an array of results
      selection = Array.isArray(selection)
        ? selection[0]
        : selection;
      if (!selection) {
        selection = latestRefmtString;
      }
      ReactDOM.render(
        <Popup initialSelectedText={selection}/>,
        document.getElementById('app'),
      );
    });
});
