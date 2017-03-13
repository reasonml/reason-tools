open Dom;

let ocamlLogo = [%bs.raw {|require('../../../../../src/images/ocamlLogo128.png')|}];

let reasonLogo = [%bs.raw {|require('../../../../../src/images/logo128.png')|}];

let addStyleSheet () => {
  let element = Document.createElement "style";
  Element.setType element "text/css";
  Element.setInnerText
    element
    {|
    .reason_tools_button.reason_tools_button.reason_tools_button {
      position: fixed;
      right: 0;
      height: 50px;
      width: 50px;
      background-color: black;
      color: white;
      font-family: monospace;
      display: flex;
      justify-content: center;
      align-items: center;
      font-weight: 900;
      opacity: 0.6;
    }
    .reason_tools_button.reason_tools_button.reason_tools_button:hover {
      opacity: 1;
      cursor: pointer;
    }
    .reason_tools_anchor {
      color: #cec47f;
    }
    .reason_tools_anchor:before {
      content: '';
      float: left;
      position: relative;
      width: 0;
      height: 50px;
      margin-top: -50px;
    }
    .reason_tools_anchor:target:after {
      content: '';
      position: relative;
      width: 4px;
      margin-left: -4px;
      height: 18px;
      float: left;
      background-color: #97B98c;
      left: -10px;
    }
  |};
  Body.appendChild element
};

let updateSyntaxSwapButton () => {
  let element = Document.getElementById "syntax-swap-button";
  let style = Element.style element;
  let reasonLogoUrl = "url(" ^ Chrome.Extension.getURL reasonLogo ^ ")";
  let ocamlLogoUrl = "url(" ^ Chrome.Extension.getURL ocamlLogo ^ ")";
  Style.setBackgroundImage
    style
    (Js.String.includes (Style.backgroundImage style) reasonLogo ? ocamlLogoUrl : reasonLogoUrl)
};

let addSyntaxSwapButton swap => {
  open Element;
  let element = Document.createElement "div";
  Style.setTop (style element) "40px";
  setId element "syntax-swap-button";
  setClassName element "reason_tools_button";
  setOnClick element (fun _ => swap `not_initial);
  Style.setBackgroundImage (style element) ("url(" ^ Chrome.Extension.getURL reasonLogo ^ ")");
  Style.setBackgroundSize (style element) "cover";
  Body.appendChild element
};

let addStyleSwapButton swap => {
  open Element;
  let element = Document.createElement "div";
  Style.setTop (style element) "90px";
  setInnerText element "</>";
  setClassName element "reason_tools_button";
  setOnClick element swap;
  Body.appendChild element
};

let addSwapButtons swapStyleSheets swapSyntax => {
  addStyleSheet ();
  addSyntaxSwapButton swapSyntax;
  addStyleSwapButton swapStyleSheets
};

let toggle swapStyleSheets swapSyntax => {
  let buttons = Document.getElementsByClassName "reason_tools_button" |> Arrayish.toArray;
  if (Array.length buttons > 0) {
    buttons |> Array.iter Element.remove
  } else {
    addSwapButtons swapStyleSheets swapSyntax
  }
};
