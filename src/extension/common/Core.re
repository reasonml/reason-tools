exception Unreachable;

let noop _ => ();

module Option = {
  let unwrapUnsafely = fun
  | Some v => v
  | None => assert false;

  let is_some = fun
  | Some _ => true
  | None => false;

  let is_none = fun
  | Some _ => false
  | None => true;

  let may f => fun
  | Some v => f v
  | None => ();

  let or_ other => fun
  | Some _ as self => self
  | None => other;

  let get_or other => fun
  | Some v => v
  | None => other;

  let map f => fun
  | Some v => Some (f v)
  | None => None;

  let map_or f other => fun
  | Some v => f v
  | None => other;

  let map_or_else f g => fun
  | Some v => f v
  | None => g ();

  let and_then f => fun
  | Some v => f v
  | None => None;
};

module MaybeArray = {
  type t 'a;

  /*
  let unwrap : t 'a => 'a = [%bs.raw {|
    function (maybeArray) {
      return Array.isArray(maybeArray)
        ? maybeArray[0]
        : maybeArray;
    }
  |}];*/
};

module Str = {
  external fromCharCode : int => string = "String.fromCharCode" [@@bs.val];
  external includes : string => string => Js.boolean = "includes" [@@bs.send];

  let isEmpty s => String.length (String.trim s) == 0;
};

module Re = {
  external make : string => Js.Re.t = "RegExp" [@@bs.new];
};

module Promise = {
  type t 'a;

  external make : (('a => unit) => ('e => unit) => unit) => t 'a = "Promise" [@@bs.new];

  external then_ : ('a => 'b) => t 'b = "then" [@@bs.send.pipe: t 'a];
  external and_then : ('a => t 'b) => t 'b = "then" [@@bs.send.pipe: t 'a];
  external catch : ('e => unit) => t 'a = "" [@@bs.send.pipe: t 'a];
  external or_ : ('e => 'b) => t 'b = "catch" [@@bs.send.pipe: t 'a]; /* non-standard name for "overload" */
  external or_else : ('e => t 'b) => t 'b = "catch" [@@bs.send.pipe: t 'a]; /* non-standard name for "overload" */

  external all : array (t 'a) => t (array 'a) = "Promise.all" [@@bs.val];
  external race : array (t 'a) => t 'b = "Promise.race" [@@bs.val]; /* unsure about what the returned promise will hold */
  external reject : 'e => t 'a = "Promise.reject" [@@bs.val];
  external resolve : 'a => t 'a = "Promise.resolve" [@@bs.val];
};

module Chrome = {
  type promise;
  type resolve;

  module Extension = {
    external getURL : string => string = "chrome.extension.getURL" [@@bs.val];
  };

  module Runtime = {
    external sendMessage : 'a => ('b => unit) = "chrome.runtime.sendMessage" [@@bs.val];
    external addMessageListener : ('a => Js.t {..} => ('b => unit) => unit) => unit = "chrome.runtime.onMessage.addListener" [@@bs.val];
  };

  module Storage = {
    module Local = {
      external get : string => ('a => unit) => unit = "chrome.storage.local.get" [@@bs.val];
      external set : Js.t {..} => unit = "chrome.storage.local.set" [@@bs.val];
    }
  };

  module Tabs = {
    external create : Js.t {. url: string } => unit = "chrome.tabs.create" [@@bs.val];

    /* TODO: Need MaybeArray to work because Chrome will return an array, but FF supposedly does not */
    /*external executeScript : Js.t {. code: string } => (MaybeArray.t (Js.t {..}) => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];*/
    external executeScript : Js.t {. code: string } => (Js.null_undefined (array string) => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];

    external sendMessage : 'id => 'a => ('b => unit) = "chrome.tabs.sendMessage" [@@bs.val];
  };

  module ContextMenus = {
    external create : Js.t {..} => unit = "chrome.contextMenus.create" [@@bs.val];
  };
};

module Message = {
  type message 'a = {
    type_: string,
    message: 'a
  };

  let send type_ message =>
    Chrome.Runtime.sendMessage { type_, message } noop;

  let sendTab id type_ message =>
    Chrome.Tabs.sendMessage id { type_, message } noop;

  let query type_ message callback =>
    Chrome.Runtime.sendMessage { type_, message } callback;

  let queryTab id type_ message callback =>
    Chrome.Tabs.sendMessage id { type_, message } callback;

  let receive type_ callback =>
    Chrome.Runtime.addMessageListener
      (fun request sender respond =>
        if (request.type_ == type_) { callback request.message sender respond; })
};

module Dom = {
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
    external setClassName : t => string => unit = "className" [@@bs.set];
    external href : t => string = "href" [@@bs.get];
    external setHref : t => string => unit = "href" [@@bs.set];
    external id : t => string = "id" [@@bs.get];
    external setId : t => string => unit = "id" [@@bs.set];
    external setInnerHTML : t => string => unit = "innerHTML" [@@bs.set];
    external innerText : t => string = "innerText" [@@bs.get];
    external setInnerText : t => string => unit = "innerText" [@@bs.set];
    external nextSibling : t => Js.null t = "nextSibling" [@@bs.get];
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

    external toReasonJsElement : t => ReasonJs.Document.element = "%identity";
  };

  module Node = {
    let _TEXT_NODE = 3;
    let _DOCUMENT_POSITION_PRECEDING = 2;
    let _DOCUMENT_POSITION_CONTAINS	= 8;

    external nodeType : Element.t => int = "" [@@bs.get];
    external nodeValue : Element.t => string = "" [@@bs.get];
    external parentNode : Element.t => Element.t = "" [@@bs.get];
    external cloneNode : Element.t => Element.t = "" [@@bs.send];
    
    external compareDocumentPosition : Element.t => Element.t => int = "" [@@bs.send];
    external insertBefore : target::Element.t => new_::Element.t => ref_::Element.t => unit = "" [@@bs.send];
  };

  module Document = {
    type t;

    external addEventListener : string => (unit => unit) => unit = "document.addEventListener" [@@bs.val];
    external createElement : string => Element.t = "document.createElement" [@@bs.val];
    external getElementsByClassName : string => Arrayish.t Element.t = "document.getElementsByClassName" [@@bs.val];
    external getElementById : string => Element.t = "document.getElementById" [@@bs.val];
    external getElementsByTagName : string => Arrayish.t Element.t = "document.getElementsByTagName" [@@bs.val];
  };

  module Location = {
    external hash : string = "window.location.hash" [@@bs.val];

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
  }
};

module Util = {
  external btoa : string => string = "window.btoa" [@@bs.val];
  external atob : string => string = "window.atob" [@@bs.val];
  external setTimeout : (unit => unit) => int => unit = "window.setTimeout" [@@bs.val];
  let classNames items =>
    items
    |> List.map (fun (name, flag) => flag ? name : "")
    |> List.filter (fun s => s !== "")
    |> String.concat " ";
};

module Hljs = {
  external registerLanguage : string => string => unit = "registerLanguage" [@@bs.module "highlight.js/lib/highlight"];
  external configure : Js.t {..} => unit = "configure" [@@bs.module "highlight.js/lib/highlight"];
  external highlightBlock : Dom.Element.t => unit = "highlightBlock" [@@bs.module "highlight.js/lib/highlight"];
};
