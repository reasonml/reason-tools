open Dom;
open Common;

Hljs.registerLanguage "ocaml" [%bs.raw "require('highlight.js/lib/languages/ocaml')"];
Hljs.configure { "classPrefix": "", "languages": [| "ocaml" |] };

if (Detect.mightBeOcamlDoc ()) {
  ConvertPage.toggle ();
};

Protocol.ToggleConversion.listen
  ConvertPage.toggle;

Protocol.RefmtSelection.listen (fun () => {
  let selection = Window.getSelection ();
  let text = selection |> Selection.toString |> normalizeText;

  Selection.removeAllRanges selection;

  Overlay.try_ text;
});
