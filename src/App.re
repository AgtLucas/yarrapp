type item = {
  id: int,
  title: string,
  completed: bool,
};

let str = ReasonReact.string;

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, _children) => {
    ...component,
    render: (_self) =>
      <div className="item">
        <label>
          <input
            _type="checkbox"
            checked=(item.completed)
          />
          (str(item.title))
        </label>
      </div>
  };
};

type state = {
  items: list(item),
};

type action =
  | AddItem;

let component = ReasonReact.reducerComponent("App");

let lastId = ref(0);
let newItem = () => {
  lastId := lastId^ + 1;
  {
    id: lastId^,
    title: "Click a button",
    completed: true
  }
};

let make = _children => {
  ...component,
  initialState: () => {
    items: [{
        id: 0,
        title: "Write some things to do!",
        completed: false,
      }]
  },
  reducer: (action, { items }) =>
    switch action {
    | AddItem => ReasonReact.Update({ items: [ newItem(), ...items ] })
    },
  render: ({ state: { items }, send }) => {
    let numItems = List.length(items);
    <div>
      <p>
        (str("What to do?"))
      </p>
      <button
        onClick=(_evt => send(AddItem))
      >
        (str("Add something"))
      </button>
      <div className="items">
        (
          List.map(item => <TodoItem key=(string_of_int(item.id)) item />, items)
            |> Array.of_list
            |> ReasonReact.array
        )
        /* Or: */
        /* (
          ReasonReact.array(Array.of_list(
            List.map(item => <TodoItem item />, items)
          ))
        ) */
      </div>
      <footer>
        (str(string_of_int(numItems) ++ " items"))
      </footer>
    </div>
  },
};
