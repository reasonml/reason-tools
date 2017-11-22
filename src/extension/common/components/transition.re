type action =
  | Style(ReactDOMRe.Style.t);

let component = ReasonReact.reducerComponent("Transition");

let make = (~before, ~after, children) => {
  ...component,
  reducer: (action, _state) =>
    switch action {
    | Style(a) => ReasonReact.Update(a)
    },
  initialState: () => before,
  didMount: (self) => {
    /*TODO: why carry the payload?*/
    ignore(Js.Global.setTimeout(self.reduce((_) => Style(after)), 0));
    ReasonReact.NoUpdate
  },
  render: ({state}) => <div style=state> (ReasonReact.arrayToElement(children)) </div>
};
