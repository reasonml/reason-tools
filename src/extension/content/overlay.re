open LocalDom;

open Common;

let renderInTheShadows = (renderer) => {
  let host = Document.createElement("div");
  let shadow = Element.attachShadow(host, {"mode": "closed"});
  let remove = () => Body.removeChild(host);
  Body.appendChild(host);
  /* React must render directly to the shadow root, otherwise onclick handlers won't work */
  ReactDOMRe.render(renderer(remove), Element.toReasonJsElement(shadow));
  /* Only after React has rendered can we attach the stylesheets, otherwise React will render over them */
  let style = Document.createElement("style");
  Element.setType(style, "text/css");
  Element.setInnerText(style, InlineStyles.stylesheet);
  Element.appendChild(shadow, createStylesheet());
  Element.appendChild(shadow, style)
};

let showOverlay = (inLang, inText, outLang, outText) =>
  renderInTheShadows(
    (remove) =>
      <InlinePopover inLang inText outLang outText close=remove open_=Protocol.OpenInTab.send />
  );

let showError = (message) => renderInTheShadows((remove) => <InlineError message close=remove />);

let try_ = (text) =>
  Protocol.Refmt.send(
    text,
    (response) =>
      switch response {
      | Error(message) => showError(message)
      | Ok({outText, inLang, outLang}) => showOverlay(inLang, text, outLang, outText)
      }
  );

let ocamlLogo = [%bs.raw {|require('../../../../../src/images/ocamlLogo128.png')|}];

let reasonLogo = [%bs.raw {|require('../../../../../src/images/logo128.png')|}];

let addStyleSheet = () => {
  let element = Document.createElement("style");
  Element.setType(element, "text/css");
  Element.setInnerText(
    element,
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
  |}
  );
  Body.appendChild(element)
};

let updateSyntaxSwapButton = () => {
  let element = Document.getElementById("syntax-swap-button");
  let style = Element.style(element);
  let logo =
    Style.backgroundImage(style) |> Js.String.includes(reasonLogo) ? ocamlLogo : reasonLogo;
  let url = Chrome.Extension.getURL(logo);
  Style.setBackgroundImage(style, {j|url($url)|j})
};

let addSyntaxSwapButton = (swap) => {
  open Element;
  let element = Document.createElement("div");
  Style.setTop(style(element), "40px");
  setId(element, "syntax-swap-button");
  setTitle(element, "Swap between OCaml and Reason syntax");
  setClassName(element, "reason_tools_button");
  setOnClick(element, (_) => swap(`not_initial));
  Style.setBackgroundImage(style(element), "url(" ++ Chrome.Extension.getURL(reasonLogo) ++ ")");
  Style.setBackgroundSize(style(element), "cover");
  Body.appendChild(element)
};

let addStyleSwapButton = (swap) => {
  open Element;
  let element = Document.createElement("div");
  Style.setTop(style(element), "90px");
  setInnerText(element, "</>");
  setTitle(element, "Swap between custom and original stylesheet");
  setClassName(element, "reason_tools_button");
  setOnClick(element, swap);
  Body.appendChild(element)
};

let addSwapButtons = (swapStyleSheets, swapSyntax) => {
  addStyleSheet();
  addSyntaxSwapButton(swapSyntax);
  addStyleSwapButton(swapStyleSheets)
};

let toggle = (swapStyleSheets, swapSyntax) => {
  let buttons = Document.getElementsByClassName("reason_tools_button") |> Arrayish.toArray;
  if (Array.length(buttons) > 0) {
    buttons |> Array.iter(Element.remove)
  } else {
    addSwapButtons(swapStyleSheets, swapSyntax)
  }
};
