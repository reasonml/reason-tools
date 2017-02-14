exception Unreachable;

module Option = {
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
  type resolve 'a = 'a => unit;

  external make : (resolve 'a => unit) => t 'a = "Promise" [@@bs.new];

  external all : array (t 'a) => t (array 'a) = "Promise.all" [@@bs.val];

  external then_ : ('a => Js.undefined 'b) => t 'b = "then" [@@bs.send.pipe: t 'a];
};

module Chrome = {
  type promise;
  type resolve;

  module Extension = {
    external getURL : string => string = "chrome.extension.getURL" [@@bs.val];
  };

  module Runtime = {
    external sendMessage : 'a => ('b => unit) = "chrome.runtime.sendMessage" [@@bs.val];
    external addMessageListener : ('a => unit => ('b => unit) => unit) => unit = "chrome.runtime.onMessage.addListener" [@@bs.val];
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

  let send type_ message callback =>
    Chrome.Runtime.sendMessage { type_, message } (fun response => callback response.message);

  let sendToTab id type_ message callback =>
    Chrome.Tabs.sendMessage id { type_, message } (fun response => response |>Js.Undefined.to_opt |> Option.map (fun r => callback r.message));

  let receive type_ callback =>
    Chrome.Runtime.addMessageListener (fun request _ respond =>
      if (request.type_ == type_) {
        callback request.message (fun response => respond { type_, message: response });
      })
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
    external setType : t => string => unit = "type" [@@bs.set];

    external setOnClick : t => (unit => unit) => unit = "onclick" [@@bs.set];

    external getAttribute : t => string => string = "getAttribute" [@@bs.send];
    external getElementsByClassName : t => string => Arrayish.t t = "" [@@bs.send];
    external getElementsByTagName : t => string => Arrayish.t t = "" [@@bs.send];
    external remove : t => unit = "remove" [@@bs.send];
    external querySelectorAll : t => string => Arrayish.t t = "" [@@bs.send];
  };

  module Node = {
    let _TEXT_NODE = 3;

    external nodeType : Element.t => int = "" [@@bs.get];
    external nodeValue : Element.t => string = "" [@@bs.get];
    external parentNode : Element.t => Element.t = "" [@@bs.get];
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
  };

  module Head = {
    external appendChild : Element.t => unit = "document.head.appendChild" [@@bs.val];
  };
};

module Util = {
  external btoa : string => string = "window.btoa" [@@bs.val];
  external atob : string => string = "window.atob" [@@bs.val];
  external setTimeout : (unit => unit) => int => unit = "window.setTimeout" [@@bs.val];
};

module Hljs = {
  external registerLanguage : string => string => unit = "registerLanguage" [@@bs.module "highlight.js"];
  external configure : Js.t {..} => unit = "configure" [@@bs.module "highlight.js"];
  external highlightBlock : Dom.Element.t => unit = "highlightBlock" [@@bs.module "highlight.js"];
};

module CM = {
  type t;

  external focus : ReactRe.reactRef => unit = "" [@@bs.send];
  let execCommand : ReactRe.reactRef => string => unit = [%bs.raw {|
    function (el, command) {
      return el.getCodeMirror().execCommand(command);
    }
  |}];
};
