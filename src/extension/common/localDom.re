module Arrayish = {
  type t('a);
  [@bs.val] external toArray : t('a) => array('a) = "Array.prototype.slice.call";
};

module DOMTokenList = {
  type t;
  [@bs.send] external contains : (t, string) => Js.boolean = "contains";
};

module Style = {
  type t;
  [@bs.get] external backgroundImage : t => string = "";
  [@bs.set] external setBackgroundImage : (t, string) => unit = "backgroundImage";
  [@bs.set] external setBackgroundSize : (t, string) => unit = "backgroundSize";
  [@bs.set] external setTop : (t, string) => unit = "top";
};

module Element = {
  type t;
  [@bs.get] external classList : t => DOMTokenList.t = "classList";
  [@bs.get] external value : Dom.element => string = "value";
  [@bs.set] external setClassName : (t, string) => unit = "className";
  [@bs.get] external href : t => string = "href";
  [@bs.set] external setHref : (t, string) => unit = "href";
  [@bs.get] external id : t => string = "id";
  [@bs.set] external setId : (t, string) => unit = "id";
  [@bs.set] external setInnerHTML : (t, string) => unit = "innerHTML";
  [@bs.get] external outerHTML : t => string = "outerHTML";
  [@bs.get] external innerText : t => string = "innerText";
  [@bs.set] external setInnerText : (t, string) => unit = "innerText";
  [@bs.set] external setTitle : (t, string) => unit = "title";
  [@bs.get] external nextSibling : t => Js.null(t) = "nextSibling";
  [@bs.get] external nextElementSibling : t => Js.null(t) = "nextElementSibling";
  [@bs.set] external setRel : (t, string) => unit = "rel";
  [@bs.get] external style : t => Style.t = "style";
  [@bs.set] external setStyle : (t, string) => unit = "style";
  [@bs.set] external setType : (t, string) => unit = "type";
  [@bs.set] external setOnClick : (t, Js.t({..}) => unit) => unit = "onclick";
  [@bs.send] external appendChild : (t, t) => unit = "";
  [@bs.send] external getAttribute : (t, string) => string = "getAttribute";
  [@bs.send] external getElementsByClassName : (t, string) => Arrayish.t(t) = "";
  [@bs.send] external getElementsByTagName : (t, string) => Arrayish.t(t) = "";
  [@bs.send] external remove : t => unit = "remove";
  [@bs.send] external querySelectorAll : (t, string) => Arrayish.t(t) = "";
  [@bs.send] external attachShadow : (t, Js.t({..})) => t = "";
  external toReasonJsElement : t => Dom.element = "%identity";
};

module Node = {
  let _TEXT_NODE = 3;
  let _DOCUMENT_POSITION_PRECEDING = 2;
  let _DOCUMENT_POSITION_CONTAINS = 8;
  [@bs.get] external nodeType : Element.t => int = "";
  [@bs.get] external nodeValue : Element.t => string = "";
  [@bs.get] external parentNode : Element.t => Element.t = "";
  [@bs.send] external cloneNode : Element.t => Element.t = "";
  [@bs.send] external compareDocumentPosition : (Element.t, Element.t) => int = "";
  [@bs.send]
  external insertBefore : (~target: Element.t, ~new_: Element.t, ~ref_: Element.t) => unit = "";
  [@bs.send] external removeChild : (Element.t, Element.t) => unit = "";
};

module Document = {
  type t;
  [@bs.val]
  external addEventListener : (string, unit => unit) => unit = "document.addEventListener";
  [@bs.val] external createElement : string => Element.t = "document.createElement";
  [@bs.val]
  external getElementsByClassName : string => Arrayish.t(Element.t) =
    "document.getElementsByClassName";
  [@bs.val] external getElementById : string => Element.t = "document.getElementById";
  [@bs.val]
  external getElementsByTagName : string => Arrayish.t(Element.t) =
    "document.getElementsByTagName";
};

module Location = {
  [@bs.val] external hash : string = "window.location.hash";
  [@bs.val] external hostname : string = "window.location.hostname";
  [@bs.val] external pathname : string = "window.location.pathname";
  [@bs.val] external reload : Js.boolean => unit = "window.location.reload";
};

module Body = {
  [@bs.val] external outerHTML : string = "document.body.outerHTML";
  [@bs.val] external appendChild : Element.t => unit = "document.body.appendChild";
  [@bs.val] external removeChild : Element.t => unit = "document.body.removeChild";
};

module Head = {
  [@bs.val] external appendChild : Element.t => unit = "document.head.appendChild";
};

module Selection = {
  type t;
  [@bs.get] external anchorNode : t => Element.t = "";
  [@bs.get] external focusNode : t => Element.t = "";
  [@bs.send] external toString : t => string = "";
  [@bs.send] external removeAllRanges : t => unit = "";
};

module Window = {
  [@bs.val] external getSelection : unit => Selection.t = "window.getSelection";
};
