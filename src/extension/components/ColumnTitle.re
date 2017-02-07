open Core;

let createElement ::name ::lang ::children () => {
  let title = lang |> Option.map_or (fun lang => "Out (" ^ lang ^ ")") "Out";

  <span style=Styles.contextTitle>
    (ReactRe.stringToElement title)
  </span>
};
