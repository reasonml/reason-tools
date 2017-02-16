open Core;

module Refmt = {
  type t = (string, string);

  external refmt : string => t = "refmt" [@@bs.module ("../../../../_build/refmt/src/app.js", "Refmt")];

  let parse: t => RefmtProtocol.response = fun
  | ("Failure", error) => Failure error
  | (conversion, outText) =>
    switch (conversion |> Js.String.split "to") {
    | [| inLang, outLang |] => Success { outText, inLang, outLang }
    | _ => Failure "Encountered some weird unknown conversion";
  };
};

RefmtProtocol.listen
	(fun request respond =>
    request.input |> Refmt.refmt
                  |> Refmt.parse
                  |> respond );

Message.receive "open"
  (fun text _ =>
    Chrome.Tabs.create { "url": ("popup.html#" ^ (Util.btoa text)) }
  );

Chrome.ContextMenus.create {
  "title": "Refmt",
  "contexts": [| "selection" |],
  "onclick": fun info tab => Message.sendToTab tab##id "refmt.selection" info##selectionText (fun _ => ())
};

Chrome.ContextMenus.create {
  "title": "Toggle",
  "contexts": [| "browser_action", "page" |],
  "onclick": fun _ tab => Message.sendToTab tab##id "toggle" () (fun _ => ())
};

/*
Chrome.ContextMenus.create {
  "title": "Report",
  "contexts": [| "browser_action" |],
  "onclick": fun () => Js.log "reported!"
};
*/
