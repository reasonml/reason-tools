exception Unreachable;

let noop = (_) => ();

external id : 'a => 'a = "%identity";

module MaybeArray = {
  type t('a);
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
  type t('a);
  [@bs.new] external make : (('a => unit, 'e => unit) => unit) => t('a) = "Promise";
  [@bs.send.pipe : t('a)] external then_ : ('a => 'b) => t('b) = "then";
  [@bs.send.pipe : t('a)] external and_then : ('a => t('b)) => t('b) = "then";
  [@bs.send.pipe : t('a)] external catch : ('e => unit) => t('a) = "";
  [@bs.send.pipe : t('a)] external or_ : ('e => 'b) => t('b) = "catch"; /* non-standard name for "overload" */
  [@bs.send.pipe : t('a)] external or_else : ('e => t('b)) => t('b) =
                          "catch"; /* non-standard name for "overload" */
  [@bs.val] external all : array(t('a)) => t(array('a)) = "Promise.all";
  [@bs.val] external race : array(t('a)) => t('b) = "Promise.race"; /* unsure about what the returned promise will hold */
  [@bs.val] external reject : 'e => t('a) = "Promise.reject";
  [@bs.val] external resolve : 'a => t('a) = "Promise.resolve";
};

module History = {
  [@bs.val]
  external replaceState : (~state: string, ~title: string, ~url: string) => unit =
    "window.history.replaceState";
};

module Util = {
  [@bs.val] external btoa : string => string = "window.btoa";
  [@bs.val] external atob : string => string = "window.atob";
  let classNames = (items) =>
    items
    |> List.map(((name, flag)) => flag ? name : "")
    |> List.filter((s) => s !== "")
    |> String.concat(" ");
};
