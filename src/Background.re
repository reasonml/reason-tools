let refmtJS jsString =>
  switch (Refmt.refmt (Js.to_string jsString)) {
  | Refmt.MLtoRE s => Js.array [|Js.string "MLtoRE", Js.string s|]
  | Refmt.REtoML s => Js.array [|Js.string "REtoML", Js.string s|]
  | Refmt.MLItoREI s => Js.array [|Js.string "MLItoREI", Js.string s|]
  | Refmt.REItoMLI s => Js.array [|Js.string "REItoMLI", Js.string s|]
  | Refmt.Failure s => Js.array [|Js.string "Failure", Js.string s|]
  };

let omnitopCBHandler jsCallback jsString => {
  jsCallback @@ Js.string jsString;()};

let omnitopJS jsString jsCallback => {
  Lwt.async @@ Omnitop.execute (Js.to_string jsString) (omnitopCBHandler jsCallback);
  ()
};

Js.export "refmt" refmtJS;

Js.export "omnitop" omnitopJS;
