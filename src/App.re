type item = {
  id: int,
  title: string,
  description: string,
  completed: bool,
};

let str = ReasonReact.string;

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, ~onToggle, _children) => {
    ...component,
    render: (_self) =>
      <div className="item">
        <label onClick=(_evt => onToggle())>
          <input
            _type="checkbox"
            checked=(item.completed)
          />
          (str(item.title))
        </label>
      </div>
  };
};

let valueFromEvent = (evt) : string => (
  evt
  |> ReactEventRe.Form.target
  |> ReactDOMRe.domElementToObj
)##value;

module Input = {
  type state = string;
  let component = ReasonReact.reducerComponent("Input");
  let make = (~onSubmit, _) => {
    ...component,
    initialState: () => "",
    reducer: (newText, _text) => ReasonReact.Update(newText),
    render: ({ state: text, send }) =>
      <input
        _type="text"
        value=text
        placeholder="Write something to do"
        onChange=(evt => send(valueFromEvent(evt)))
        onKeyDown=(evt =>
          if (ReactEventRe.Keyboard.key(evt) == "Enter") {
            onSubmit(text);
            (() => send(""))()
          }
        )
      />
  }
};

type formValues = {
  title: string,
  description: string,
};

module Form = {
  type state = {
    values: record(formValues),
  };
  let component = ReasonReact.reducerComponent("Form");
  let make = (~onSubmit, _) => {
    ...component,
    initialState: () => {
      title: "",
      description: "",
    },
    reducer: (newText, _) => ReasonReact.Update(newText),
    render: ({ state: { title, description }, send }) =>
      <form>
        <input
          _type="text"
          value=title,
          placeholder="Title"
          onChange=(evt => send(valueFromEvent(evt)))
        />
        <input
          _type="text"
          value=description,
          placeholder="Description"
          onChange=(evt => send(valueFromEvent(evt)))
        />
        <button
          onClick=(() => send(onSubmit({ title, description })))
        >
          (str("Create"))
        </button>
      </form>
  }
}

type state = {
  items: list(item),
};

type action =
  | AddItem(record)
  | ToggleItem(int);

let component = ReasonReact.reducerComponent("App");

let lastId = ref(0);
let newItem = ({ text , description }) => {
  lastId := lastId^ + 1;
  {
    id: lastId^,
    title: text,
    description: description,
    completed: false
  }
};

let make = _children => {
  ...component,
  initialState: () => {
    items: [{
        id: 0,
        title: "Write some things to do!",
        description: "Whatever...",
        completed: false,
      }]
  },
  reducer: (action, { items }) =>
    switch action {
    | AddItem({ text, description }) => ReasonReact.Update({ items: [ newItem({ text, description }), ...items ] })
    | ToggleItem(id) =>
      let items = List.map(
        item =>
          item.id === id ?
            { ...item, completed: !item.completed } : item,
        items
      );
      ReasonReact.Update({ items: items })
    },
  render: ({ state: { items }, send }) => {
    let numItems = List.length(items);
    <div>
      <p>
        (str("What to do?"))
      </p>
      /* <Input onSubmit=(({ text, description }) => send(AddItem({ text, description }))) /> */
      <Form
        onSubmit=(({ title, description }) => send(AddItem({ text, description })))
      />
      <div className="items">
        /* (
          List.map(
            item =>
              <TodoItem
                item
                key=(string_of_int(item.id))
                onToggle=(() => send(ToggleItem(item.id)))
              />,
            items
          )
            |> Array.of_list
            |> ReasonReact.array
        ) */
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
