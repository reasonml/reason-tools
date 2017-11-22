let component = ReasonReact.statelessComponent("OpenButton");

let make = (~onClick, ~style=?, _) => {
  ...component,
  render: (_) =>
    <span className="open-button" ?style onClick>
      <svg
        viewBox="0 0 748 1024"
        style=
          (ReactDOMRe.Style.make(~width="100%", ~height="100%", ())) /*xmlns="http://www.w3.org/2000/svg"*/>
        /* Unsupported attribute */

          <path
            d="M640 768H128V257.90599999999995L256 256V128H0v768h768V576H640V768zM384 128l128 128L320 448l128 128 192-192 128 128V128H384z"
          />
        </svg>
    </span>
};
