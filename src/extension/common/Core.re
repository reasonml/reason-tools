exception Unreachable;

let noop _ => ();
external id : 'a => 'a = "%identity";

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

module Hisory = {
  external replaceState :
    state::string =>
    title::string =>
    url::string => unit = "window.history.replaceState" [@@bs.val];
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
