let iconStyle = ReactDOMRe.Style.make fontSize::"16vh" ();
let textStyle = ReactDOMRe.Style.make fontSize::"3vh" whiteSpace::"nowrap" marginTop::"4vh" ();

let make ::show ::text="Text copied to clipboard" ::style=? _ => {
  ...(ReasonReact.statelessComponent "CopyConfirmation"),

  render: fun _ _ =>
    show ?
      <div style=?style>
        <div style=iconStyle> (ReactRe.stringToElement (Js.String.fromCodePoint 0x2398)) </div>
        <div style=textStyle> (ReactRe.stringToElement text) </div>
      </div> :
      ReactRe.nullElement
};