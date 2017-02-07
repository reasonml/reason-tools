exception Unreachable;

module Option = {
  let get_or other => fun
  | Some x => x
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

  external all : list (t _) => t unit = "Promise.all" [@@bs.val];

  external then_ : t 'a => ('a => Js.undefined 'b) => t 'b = "then" [@@bs.send];
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
    external executeScript : Js.t {. code: string } => (MaybeArray.t (Js.t {..}) => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];
  };
};

module Dom = {
  module NotArray = {
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
    external setInnerHTML : t => string => unit = "innerHTML" [@@bs.set];
    external innerText : t => string = "innerText" [@@bs.get];
    external setInnerText : t => string => unit = "innerText" [@@bs.set];
    external nextSibling : t => Js.null t = "nextSibling" [@@bs.get];
    external setRel : t => string => unit = "rel" [@@bs.set];
    external style : t => Style.t = "style" [@@bs.get];
    external setType : t => string => unit = "type" [@@bs.set];

    external setOnClick : t => (unit => unit) => unit = "onclick" [@@bs.set];

    external getAttribute : t => string => string = "getAttribute" [@@bs.send];
    external getElementsByTagName : t => string => NotArray.t t = "getElementsByTagName" [@@bs.send];
    external remove : t => unit = "remove" [@@bs.send];
    external querySelectorAll : t => string => NotArray.t t = "querySelectorAll" [@@bs.send];
  };

  module Node = {
    let _TEXT_NODE = 3;

    external nodeType : Element.t => int = "nodeType" [@@bs.get];
    external nodeValue : Element.t => string = "nodeValue" [@@bs.get];
    external parentNode : Element.t => Element.t = "parentNode" [@@bs.get];
  };

  module Document = {
    type t;

    external createElement : string => Element.t = "document.createElement" [@@bs.val];
    external getElementsByClassName : string => NotArray.t Element.t = "document.getElementsByClassName" [@@bs.val];
    external getElementsByTagName : string => NotArray.t Element.t = "document.getElementsByTagName" [@@bs.val];
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

module Hljs = {
  external registerLanguage : string => string => unit = "registerLanguage" [@@bs.module "highlight.js"];
  external configure : Js.t {..} => unit = "configure" [@@bs.module "highlight.js"];
  external highlightBlock : Dom.Element.t => unit = "highlightBlock" [@@bs.module "highlight.js"];
};
