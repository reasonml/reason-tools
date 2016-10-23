require('./Popup.html');
require('./logo.png');

import SyntaxHighlighter from 'react-syntax-highlighter';
import {ocean} from 'react-syntax-highlighter/dist/styles';
import React from 'react';
import ReactDOM from 'react-dom';
import {popup, popupColumn, popupInNOut, popupContext} from './styles';

const getLangList = (conversionType) => {
  if (!conversionType) {
    return ['',''];
  }
  return conversionType.split('to');
}

class Popup extends React.Component {
  state = {out: '', inLang: '', outLang: ''};

  constructor(props) {
    super(props);
    if (this.props.initialSelectedText) {
      chrome.runtime.sendMessage(
        {in: this.props.initialSelectedText},
        ({out: [conversionType, out]}) => {
          const [inLang, outLang] = getLangList(conversionType);
          this.setState({out, inLang, outLang});
      });
    }
  }

  componentDidMount() {
    this._in && this._in.focus();
  }

  render() {
    return (
      <div style={popup}>
        <div style={popupColumn}>
          <h1 style={popupContext}>
            In{this.state.inLang ? ` (${this.state.inLang})` : ''}
          </h1>
          <textarea
            id="in"
            ref={(ref) => this._in = ref}
            defaultValue={this.props.initialSelectedText}
            onChange={this._handleChange}
            style={popupInNOut}
          />
        </div>
        <div style={popupColumn}>
          <h1 style={popupContext}>
            Out{this.state.outLang ? ` (${this.state.outLang})` : ''}
          </h1>
          <SyntaxHighlighter
            customStyle={popupInNOut}
            id="out"
            language={this.state.outLang === 'ML' ? 'ocaml' : 'javascript'}
            style={ocean}>
            {this.state.out}
          </SyntaxHighlighter>
        </div>
      </div>
    );
  }

  _handleChange = (event) => {
    chrome.runtime.sendMessage(
      {in: event.target.value},
      ({out: [conversionType, out]}) => {
        const [inLang, outLang] = getLangList(conversionType);
        this.setState({
          out,
          inLang,
          outLang,
        });
    });
  }
}

document.addEventListener('DOMContentLoaded', () => {
  chrome.tabs.query({active: true, currentWindow: true}, (tabs) => {
    chrome.tabs.sendMessage(tabs[0].id, {getSelectedText: true}, (response) => {
      ReactDOM.render(
        <Popup initialSelectedText={response.selectedText}/>,
        document.getElementById('app'),
      );
    });
  });
});
