let component = ReasonReact.statelessComponent("Essay");

let str = ReasonReact.string;

let make = (~title, ~description, _children) => {
  ...component,
  render: _self => {
    <div>
      <h1>(str(title))</h1>
      <p>(str(description))</p>
    </div>
  },
};
