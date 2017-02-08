open Core;

let formatTitle name lang => name ^ " (" ^ lang ^ ")";

let createElement ::name ::lang ::children () => {
  let title = lang |> Option.map_or (formatTitle name) name;

  <span style=Styles.contextTitle>
    (ReactRe.stringToElement title)
  </span>
};
