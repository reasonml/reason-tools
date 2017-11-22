type state = {style: ReactDOMRe.Style.t};

let component = ReasonReact.statefulComponent("Transition");

let make = (~before, ~after, children) => {
  ...component,
  initialState: () => {style: before},
  didMount: (self) => {
    Core.Util.setTimeout(self.update((_, _) => ReasonReact.Update({style: after})), 0);
    ReasonReact.NoUpdate
  },
  render: ({state}) => <div style=state.style> (ReasonReact.arrayToElement(children)) </div>
};
