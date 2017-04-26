module Arrayish = {
  type t 'a;
  external toArray : t 'a => array 'a = "Array.prototype.slice.call" [@@bs.val];
};

module DOMTokenList = {
  type t;
  external contains : t => string => Js.boolean = "contains" [@@bs.send];
};

module Style = {
  type t;
  external backgroundImage : t => string = "" [@@bs.get];
  external setBackgroundImage : t => string => unit = "backgroundImage" [@@bs.set];
  external setBackgroundSize : t => string => unit = "backgroundSize" [@@bs.set];
  external setTop : t => string => unit = "top" [@@bs.set];
};

module Element = {
  type t;
  external classList : t => DOMTokenList.t = "classList" [@@bs.get];
  external value : Dom.element => string = "value" [@@bs.get];
  external setClassName : t => string => unit = "className" [@@bs.set];
  external href : t => string = "href" [@@bs.get];
  external setHref : t => string => unit = "href" [@@bs.set];
  external id : t => string = "id" [@@bs.get];
  external setId : t => string => unit = "id" [@@bs.set];
  external setInnerHTML : t => string => unit = "innerHTML" [@@bs.set];
  external innerText : t => string = "innerText" [@@bs.get];
  external setInnerText : t => string => unit = "innerText" [@@bs.set];
  external setTitle : t => string => unit = "title" [@@bs.set];
  external nextSibling : t => Js.null t = "nextSibling" [@@bs.get];
  external nextElementSibling : t => Js.null t = "nextElementSibling" [@@bs.get];
  external setRel : t => string => unit = "rel" [@@bs.set];
  external style : t => Style.t = "style" [@@bs.get];
  external setStyle : t => string => unit = "style" [@@bs.set];
  external setType : t => string => unit = "type" [@@bs.set];
  external setOnClick : t => (Js.t {..} => unit) => unit = "onclick" [@@bs.set];
  external appendChild : t => t => unit = "" [@@bs.send];
  external getAttribute : t => string => string = "getAttribute" [@@bs.send];
  external getElementsByClassName : t => string => Arrayish.t t = "" [@@bs.send];
  external getElementsByTagName : t => string => Arrayish.t t = "" [@@bs.send];
  external remove : t => unit = "remove" [@@bs.send];
  external querySelectorAll : t => string => Arrayish.t t = "" [@@bs.send];
  external attachShadow : t => Js.t {..} => t = "" [@@bs.send];
  external toReasonJsElement : t => Dom.element = "%identity";
};

module Node = {
  let _TEXT_NODE = 3;
  let _DOCUMENT_POSITION_PRECEDING = 2;
  let _DOCUMENT_POSITION_CONTAINS = 8;
  external nodeType : Element.t => int = "" [@@bs.get];
  external nodeValue : Element.t => string = "" [@@bs.get];
  external parentNode : Element.t => Element.t = "" [@@bs.get];
  external cloneNode : Element.t => Element.t = "" [@@bs.send];
  external compareDocumentPosition : Element.t => Element.t => int = "" [@@bs.send];
  external insertBefore : target::Element.t => new_::Element.t => ref_::Element.t => unit =
    "" [@@bs.send];
  external removeChild : Element.t => Element.t => unit = "" [@@bs.send];
};

module Document = {
  type t;
  external addEventListener : string => (unit => unit) => unit =
    "document.addEventListener" [@@bs.val];
  external createElement : string => Element.t = "document.createElement" [@@bs.val];
  external getElementsByClassName : string => Arrayish.t Element.t =
    "document.getElementsByClassName" [@@bs.val];
  external getElementById : string => Element.t = "document.getElementById" [@@bs.val];
  external getElementsByTagName : string => Arrayish.t Element.t =
    "document.getElementsByTagName" [@@bs.val];
};

module Location = {
  external hash : string = "window.location.hash" [@@bs.val];
  external hostname : string = "window.location.hostname" [@@bs.val];
  external pathname : string = "window.location.pathname" [@@bs.val];
  external reload : Js.boolean => unit = "window.location.reload" [@@bs.val];
};

module Body = {
  external outerHTML : string = "document.body.outerHTML" [@@bs.val];
  external appendChild : Element.t => unit = "document.body.appendChild" [@@bs.val];
  external removeChild : Element.t => unit = "document.body.removeChild" [@@bs.val];
};

module Head = {
  external appendChild : Element.t => unit = "document.head.appendChild" [@@bs.val];
};

module Selection = {
  type t;
  external anchorNode : t => Element.t = "" [@@bs.get];
  external focusNode : t => Element.t = "" [@@bs.get];
  external toString : t => string = "" [@@bs.send];
  external removeAllRanges : t => unit = "" [@@bs.send];
};

module Window = {
  external getSelection : unit => Selection.t = "window.getSelection" [@@bs.val];
};
