open LocalDom;
open Common;

let renderInTheShadows renderer => {
  let host = Document.createElement "div";
  let shadow = Element.attachShadow host {"mode": "closed"};
  let remove () => Body.removeChild host;

  Body.appendChild host;

  /* React must render directly to the shadow root, otherwise onclick handlers won't work */
  ReactDOMRe.render (renderer remove) (Element.toReasonJsElement shadow);

  /* Only after React has rendered can we attach the stylesheets, otherwise React will render over them */
  let style = Document.createElement "style";
  Element.setType style "text/css";
  Element.setInnerText style InlineStyles.stylesheet;
  Element.appendChild shadow (createStylesheet ());
  Element.appendChild shadow style
};

let showOverlay inLang inText outLang outText =>
  renderInTheShadows (fun remove =>
    <InlinePopover
      inLang
      inText
      outLang
      outText
      close=remove
      open_=Protocol.OpenInTab.send
    />);

let showError message =>
  renderInTheShadows (fun remove => <InlineError message close=remove />);

let try_ text =>
  Protocol.Refmt.send
    text
    (
      fun response =>
        switch response {
        | Error message => showError message
        | Ok {outText, inLang, outLang} => showOverlay inLang text outLang outText
        }
    );
