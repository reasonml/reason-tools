open LocalDom;

Hljs.registerLanguage "ocaml" [%bs.raw "require('highlight.js/lib/languages/ocaml')"];
Hljs.configure {"classPrefix": "", "languages": [|"ocaml"|]};

Protocol.Storage.queryDisabled (fun disabled => {
  if (not disabled && Detect.shouldConvert ()) {
    ConvertPage.toggle ()
  };
});

Protocol.ToggleConversion.listen ConvertPage.toggle;

Protocol.RefmtSelection.listen (
  fun () => {
    let selection = Window.getSelection ();
    let text = selection |> Selection.toString;
    Selection.removeAllRanges selection;
    Overlay.try_ text
  }
);
