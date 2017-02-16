open Core;

let formatTitle name lang => name ^ " (" ^ lang ^ ")";

let createElement ::name ::lang children::_ () => {
  let title = lang |> Option.map_or (formatTitle name) name;

  <span style=PopupStyles.contextTitle>
    (ReactRe.stringToElement title)
  </span>
};
