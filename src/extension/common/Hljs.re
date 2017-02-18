external registerLanguage : string => string => unit = "registerLanguage" [@@bs.module "highlight.js/lib/highlight"];
external configure : Js.t {..} => unit = "configure" [@@bs.module "highlight.js/lib/highlight"];
external highlightBlock : Dom.Element.t => unit = "highlightBlock" [@@bs.module "highlight.js/lib/highlight"];
