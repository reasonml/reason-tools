require('./Popup.html');
require('./logo19.png');
require('./logo38.png');
require('./logo128.png');

import CopyToClipboard from 'react-copy-to-clipboard';
import SyntaxHighlighter from 'react-syntax-highlighter';
import {ocean} from 'react-syntax-highlighter/dist/styles';
import React from 'react';
import ReactDOM from 'react-dom';

import {
  popup,
  popupColumn,
  popupInNOut,
  popupContext,
  savedBadge,
  saveLink,
} from './styles';

const getLangList = (conversionType) => {
  if (!conversionType) {
    return ['',''];
  }
  return conversionType.split('to');
}

class Popup extends React.Component {
  state = {copy: false, in: '', out: '', inLang: '', outLang: ''};

  componentDidMount() {
    if (this.props.initialSelectedText) {
      this._refmt(this.props.initialSelectedText);
    }
    this._in && this._in.focus();
  }

  render() {
    return (
      <div style={popup}>
        <div style={popupColumn}>
          <h1 style={popupContext}>
            In{this.state.inLang && this.state.in
                ? ` (${this.state.inLang})`
                : ''}
          </h1>
          <textarea
            id="in"
            ref={(ref) => this._in = ref}
            value={this.state.in}
            onChange={this._handleChange}
            style={popupInNOut}
          />
        </div>
        <div style={popupColumn}>
          <h1 style={popupContext}>
            Out{
              this.state.outLang && this.state.out
                ? ` (${this.state.outLang})`
                : ''}
            <CopyToClipboard text={this.state.out} onCopy={this._handleCopy}>
              <a style={saveLink} href="#">copy</a>
            </CopyToClipboard>
          </h1>
          <SyntaxHighlighter
            customStyle={popupInNOut}
            id="out"
            language={this.state.outLang === 'ML' ? 'ocaml' : 'javascript'}
            style={ocean}>
            {this.state.out}
          </SyntaxHighlighter>
          {this.state.copy && <div style={savedBadge}>{'\u2713 '}</div>}
        </div>
      </div>
    );
  }

  _handleCopy = () => {
    this.setState({copy: true});
    setTimeout(() => this.setState({copy: false}), 2500);
  }

  _handleChange = (event) => {
    const value = event.target.value;
    this._refmt(value);
  }

  _refmt(value) {
    this.setState({in: value});
    // this isn't guaranteed to be sync or speedy, so
    // don't set this.state.in here, since it could cause lag.
    chrome.runtime.sendMessage({in: value},
      ({out: [conversionType, out]}) => {
        const [inLang, outLang] = getLangList(conversionType);
        this.setState({out, inLang, outLang});
      }
    );
  }
}

document.addEventListener('DOMContentLoaded', () => {
  chrome.tabs.query({active: true, currentWindow: true}, (tabs) => {
    chrome.tabs.sendMessage(tabs[0].id, {getSelectedText: true}, (response) => {
      ReactDOM.render(
        <Popup initialSelectedText={response ? response.selectedText : null}/>,
        document.getElementById('app'),
      );
    });
  });
});
