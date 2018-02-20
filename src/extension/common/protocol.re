open Rebase;

open Common;

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
  type payloadSerialized = {
    outText: string,
    inLang: string,
    outLang: string
  };
  type payload = {
    outText: string,
    inLang: language,
    outLang: language
  };
  type response = result(payload, string);
  /* Bucklescript's variant tags will be erased when serialized, so we have to manually serialize the response
   */
  let serialize: result(payload, string) => (int, payloadSerialized) =
    fun
    | Error(error) => (0, {outText: error, inLang: "", outLang: ""})
    | Ok(payload) => (
        1,
        {
          outText: payload.outText,
          inLang: stringOfLanguage(payload.inLang),
          outLang: stringOfLanguage(payload.outLang)
        }
      );
  let deserialize: ((int, payloadSerialized)) => result(payload, string) =
    fun
    | (0, {outText: error}) => Error(error)
    | (1, payload) =>
      Ok({
        outText: payload.outText,
        inLang: languageOfString(payload.inLang),
        outLang: languageOfString(payload.outLang)
      })
    | _ => raise(DeserializationFail);
  let send =
      (
        text,
        ~inLang=RefmtShared.UnknownLang,
        ~inType=RefmtShared.UnknownType,
        ~outLang=RefmtShared.UnknownLang,
        cb
      ) =>
    Message.query(
      "refmt:refmt",
      {input: text |> normalizeText |> untoplevel, inLang, inType, outLang},
      (response) => cb(deserialize(response))
    );
  let listen: ((request, response => unit) => unit) => unit =
    (cb) =>
      Message.receive(
        "refmt:refmt",
        (request, _, respond) => cb(request, (r) => r |> serialize |> respond)
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
      (response) => response##disabled |> Js.Undefined.to_opt |> Option.getOr(false) |> callback
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
