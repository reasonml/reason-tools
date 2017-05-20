module BrowserAction = {
  external setIcon : Js.t {..} => unit = "chrome.browserAction.setIcon" [@@bs.val];
};

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
  };

  external addChangeListener : (Js.Dict.t (Js.t {. newValue: 'a, oldValue: 'a }) => string => unit) => unit = "chrome.storage.onChanged.addListener" [@@bs.val];
};

module Commands = {
  external addListener: (string => unit) => unit = "chrome.commands.onCommand.addListener" [@@bs.val];
};

module Tabs = {
  type tabId;
  /*
  type tab = Js.t {.
    id: tabId
  };
  */

  external create : Js.t {. url: string } => unit = "chrome.tabs.create" [@@bs.val];
  external update : int => Js.t {..} => unit = "chrome.tabs.update" [@@bs.val];

  /* TODO: Need MaybeArray to work because Chrome will return an array, but FF supposedly does not */
  /*external executeScript : Js.t {. code: string } => (MaybeArray.t (Js.t {..}) => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];*/
  external executeScript : Js.t {. code: string } => (Js.null_undefined (array string) => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];
  external executeScriptFile : Js.t {. file: string } => (unit => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];
  /* TODO: could use bs.ginore here? */

  external sendMessage : tabId => 'a => ('b => unit) = "chrome.tabs.sendMessage" [@@bs.val];
  external query: Js.t {..} => (array (Js.t {. url: string, id:int}) => unit) => unit = "chrome.tabs.query" [@@bs.val];
};

module ContextMenus = {
  type id;
  /*
  type config = Js.t {.
    title: string,
    context: array string,
    onclick: (unit => Tabs.tab => unit)
  };
  */

  external create : /*config*/ Js.t {..} => id = "chrome.contextMenus.create" [@@bs.val];
  external update : id => /*config*/ Js.t {..} => unit = "chrome.contextMenus.update" [@@bs.val];
};
