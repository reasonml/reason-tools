import SyntaxHighlighter from 'react-syntax-highlighter';
import {ocean} from 'react-syntax-highlighter/dist/styles';
import React from 'react';
import ReactDOM from 'react-dom';

const getLangList = (conversionType) => {
  if (!conversionType) {
    return ["",""];
  }
  return conversionType.split('to');
}

class Popup extends React.Component {
  state = {out: "", inLang: "", outLang: ""};
  render() {
    return (
      <div id="popup">
        <div>
          <h1 id="in-context">In{this.state.inLang ? ` (${this.state.inLang})` : ''}</h1>
          <textarea id="in" onChange={this._handleChange}/>
        </div>
        <div>
          <h1 id="out-context">Out{this.state.outLang ? ` (${this.state.outLang})` : ''}</h1>
          <SyntaxHighlighter
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

document.addEventListener('DOMContentLoaded', function() {
  ReactDOM.render(<Popup />, document.getElementById('app'));
});
