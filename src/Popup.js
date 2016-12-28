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
  saveLink,
} from './styles';
import getLangList from './getLangList';

class Popup extends React.Component {
  state = {copy: false, in: '', out: '', inLang: null, outLang: null};

  componentDidMount() {
    if (this.props.initialSelectedText) {
      this._refmt(this.props.initialSelectedText);
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
            defaultValue={this.props.initialSelectedText}
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
            Out{
              hasConverted && this.state.outLang
                ? ` (${this.state.outLang})`
                : null}
            <CopyToClipboard text={this.state.out} onCopy={this._handleCopy}>
              <a style={saveLink} href="#">copy</a>
            </CopyToClipboard>
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

  _refmt = (value) => {
    this.setState({in: value});
    // this isn't guaranteed to be sync or speedy, so
    // don't set this.state.in here, since it could cause lag.
    chrome.runtime.sendMessage({type: ['refmt'], payload: value},
      ({refmt: [conversionType, out, ...rest]}) => {
        const [inLang, outLang] = getLangList(conversionType);
        this.setState({out, inLang, outLang});
      }
    );
  }
}

document.addEventListener('DOMContentLoaded', () => {
    chrome.tabs.executeScript(
      {code: 'window.getSelection().toString();'},
      (selection) => {
        // in ff you get a single result, chrome returns an array of results
        selection = Array.isArray(selection)
          ? selection[0]
          : selection;
        ReactDOM.render(
          <Popup initialSelectedText={selection}/>,
          document.getElementById('app'),
        );
      },
    );
});
