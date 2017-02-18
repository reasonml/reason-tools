open Dom;
open Common;

let open_ inLang inText outLang outText => {
  let host = Document.createElement "div";
  let shadow = Element.attachShadow host { "mode": "closed" };

  Body.appendChild host;

  /* React must render directly to the shadow root, otherwise onclick handlers won't work */
  ReactDOMRe.render
    <InlinePopover
      inLang inText
      outLang outText
      close=(fun () => Body.removeChild host)
      open_=Protocol.OpenInTab.send
    />
    (Element.toReasonJsElement shadow);

  /* Only after React has rendered can we attach the stylesheets, otherwise React will render over them */
  Element.appendChild shadow (createStylesheet ());
  let style = Document.createElement "style";
  Element.setType style "text/css";
  Element.setInnerText style InlineStyles.stylesheet;
  Element.appendChild shadow style;
};

let try_ text =>
  Protocol.Refmt.send
    (normalizeText text)
    (fun response => {
      switch response {
        | Error _ => () /* TODO */
        | Ok { outText, inLang, outLang } =>
          open_ inLang text outLang outText;
      };
    });
