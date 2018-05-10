open Common;
open Result;

type language = RefmtShared.language;

type codeType = RefmtShared.codeType;

let stringOfLanguage = RefmtShared.stringOfLanguage;

let languageOfString = RefmtShared.languageOfString;

let stringOfType = RefmtShared.stringOfType;

let stringOfLanguageHuman = (lang: language) =>
  switch lang {
  | ML => "ML"
  | RE => "RE"
  | REO => "RE v1"
  | UnknownLang => "Unkown"
  };

module Refmt = {
  exception DeserializationFail;
  type request = {
    input: string,
    inLang: language,
    inType: codeType,
    outLang: language
  };
  [@bs.deriving jsConverter]
  type payload = {
    outText: string,
    inLang: language,
    outLang: language
  };
  type response = Result.t(payload, string);
  let responseToJs = x => {
    Js.log2("hdon sez responseToJs got:", x);
    switch (x) {
    | Result.Ok(x) => Obj.magic(("Ok", payloadToJs(x)))
    | Result.Error(x) => Obj.magic(("Error", x))
    };
  };
  let responseFromJs:Js.Json.t=>response = x => {
    Js.log2("hdon sez responseFromJs got:", x);
    switch(Obj.magic(x)) {
    | ("Ok", x) =>    Result.Ok(payloadFromJs(Obj.magic(x)))
    | ("Error", x) => Result.Error(Obj.magic(x))
    };
  };
  /* Bucklescript's variant tags will be erased when serialized, so we have to manually serialize the response
   */
  let send =
      (
        text,
        ~inLang=RefmtShared.UnknownLang,
        ~inType=RefmtShared.UnknownType,
        ~outLang=RefmtShared.UnknownLang,
        cb:(response)=>unit
      ) =>
    Message.query(
      "refmt:refmt",
      {input: text |> normalizeText |> untoplevel, inLang, inType, outLang},
      (response) => cb(responseFromJs(response))
    );
  let listen: ((request, response => unit) => unit) => unit =
    (cb) =>
      Message.receive(
        "refmt:refmt",
        (request, _, respond) => cb(request, (r) => {
          Js.log2("hdon sez listener responding with:", r);
          r |> responseToJs |> respond;
        })
      );
};

module OpenInTab = {
  let send = (text) => Message.send("background:open", text);
  let listen = (callback) => Message.receive("background:open", (text, _, _) => callback(text));
};

module ToggleConversion = {
  let send = (tabId) => Message.sendTab(tabId, "content:toggle", ());
  let listen = (callback) => Message.receive("content:toggle", (_, _, _) => callback());
};

module RefmtSelection = {
  let send = (tabId) => Message.sendTab(tabId, "content:refmt-selection", ());
  let listen = (callback) => Message.receive("content:refmt-selection", (_, _, _) => callback());
};

module LoadScripts = {
  let send = () => Message.send("background:load-content-scripts", ());
  let listen = (callback) =>
    Message.receive(
      "background:load-content-scripts",
      (_, sender, _) => callback(sender##tab##id)
    );
};

module NotifyLoaded = {
  let send = (tabId) => Message.sendTab(tabId, "content:notify-loaded", ());
  let listen = (callback) => Message.receive("content:notify-loaded", (_, _, _) => callback());
};

module QueryLoaded = {
  let query = (tabId, callback) => Message.queryTab(tabId, "content:query-loaded", (), callback);
  let listen = (callback) =>
    Message.receive("content:query-loaded", (_, _, respond) => respond(callback()));
};

module Storage = {
  let queryDisabled = (callback: bool => unit) =>
    Chrome.Storage.Local.get(
      "disabled",
      (response) => response##disabled |> Js.Undefined.toOption |> Js.Option.getWithDefault(false) |> callback
    );
  let setDisabled = (value: bool) => Chrome.Storage.Local.set({"disabled": value});
  let onDisabledChanged = (callback: bool => unit) =>
    Chrome.Storage.addChangeListener(
      (changes, _) =>
        switch (Js.Dict.get(changes, "disabled")) {
        | Some(change) => callback(change##newValue)
        | None => ()
        }
    );
  let queryLatestInput = (callback: option(string) => unit) =>
    Chrome.Storage.Local.get(
      "latestRefmtString",
      (response) => response##latestRefmtString |> Js.Null_undefined.to_opt |> callback
    );
  let setLatestInput = (value: string) => Chrome.Storage.Local.set({"latestRefmtString": value});
};
