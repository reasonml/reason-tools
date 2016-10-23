import Refmt from '../_build/refmt/src/app.js';
import SyntaxHighlighter from 'react-syntax-highlighter';
import { ocean } from 'react-syntax-highlighter/dist/styles';
import React from 'react';
import ReactDOM from 'react-dom';

class Popup extends React.Component {
  state = {in: "", out: ""};
  render() {
    return (
      <div id="popup">
        <div>
          <h1 id="in-context">In</h1>
          <textarea id="in" value={this.state.in} onChange={this._handleChange}/>
        </div>
        <div>
          <h1 id="out-context">Out</h1>
          <SyntaxHighlighter
            id="out"
            language='javascript'
            style={ocean}>
            {this.state.out}
          </SyntaxHighlighter>
        </div>
      </div>
    );
  }

  _handleChange = (event) => {
    this.setState({
      in: event.target.value,
      out: Refmt.refmt(event.target.value),
    });
  }
}

document.addEventListener('DOMContentLoaded', function() {
  ReactDOM.render(<Popup />, document.getElementById('app'));
});
