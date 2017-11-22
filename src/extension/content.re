open LocalDom;

Hljs.registerLanguage("ocaml", [%bs.raw "require('highlight.js/lib/languages/ocaml')"]);

Hljs.configure({"classPrefix": "", "languages": [|"ocaml"|]});

Protocol.Storage.queryDisabled(
  (disabled) =>
    if (! disabled && Detect.shouldConvert()) {
      ConvertPage.toggle()
    }
);

Protocol.ToggleConversion.listen(ConvertPage.toggle);

Protocol.RefmtSelection.listen(
  () => {
    let selection = Window.getSelection();
    let text = selection |> Selection.toString;
    Selection.removeAllRanges(selection);
    Overlay.try_(text)
  }
);
