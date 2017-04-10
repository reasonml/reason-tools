open Rebase;

open LocalDom;

open Common;

type saveState = {mutable stylesheets: list Element.t};

let savedState = {stylesheets: [createStylesheet ()]};

let swapStyleSheets _ => {
  let stylesheets =
    getElementsByTagName None "link" |>
    List.filter (fun link => Element.getAttribute link "rel" == "stylesheet");
  stylesheets |> List.iter Element.remove;
  savedState.stylesheets |> List.iter Head.appendChild;
  savedState.stylesheets = stylesheets
};

let readjustViewport () =>
  if (not (Str.isEmpty Location.hash)) {
    [%bs.raw {| window.location.href = window.location.href |}]
  };

type swapState = {mutable remaining: int};

let doListing mode state listing => {
  open Retrieve;
  let {els, text, replace} = listing;
  Protocol.Refmt.send
    (normalizeText text)
    (
      fun response => {
        switch response {
        | Error _ => () /* TODO */
        | Ok {outText} => Replace.replaceListing els outText replace
        };
        /* we're in an async callback, so keep track of when we're finished by keeping count */
        state.remaining = state.remaining - 1;
        if (mode == `initial && state.remaining <= 0) {
          /* when we're done, the DOM has most likely been shifted,
           * so we need to go back to where we're supposed to be */
          readjustViewport ()
        }
      }
    )
};

let swapSyntax mode => {
  let listings = Retrieve.getListings ();
  let state = {remaining: List.length listings};
  listings |> List.iter (doListing mode state);
  UI.updateSyntaxSwapButton ()
};

let toggle () => {
  swapStyleSheets ();
  UI.toggle swapStyleSheets swapSyntax;
  swapSyntax `initial
};
