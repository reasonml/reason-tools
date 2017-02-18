open Rebase;

let formatTitle name lang => name ^ " (" ^ lang ^ ")";

let createElement ::name ::lang children::_ () => {
  let title = lang |> Option.mapOr (formatTitle name) name;

  <span style=PopupStyles.contextTitle>
    (ReactRe.stringToElement title)
  </span>
};
