open Core;

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
