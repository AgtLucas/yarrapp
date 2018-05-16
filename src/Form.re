let component = ReasonReact.statelessComponent("Form");

let str = ReasonReact.string;

let make = (
  ~title,
  ~text,
  ~onChangeTitle,
  ~onChangeText,
  ~createEssay,
  _children
) => {
  ...component,
  render: _self => {
    <div>
      <form
        onSubmit=createEssay
      >
        <label>
          (str("Title"))
          <input
            _type="text"
            value=title
            placeholder="e.g.: My awesome title!"
            onChange=onChangeTitle
          />
        </label>
        <label>
          (str("Description"))
          <input
            _type="text"
            value=text
            placeholder="e.g.: This is an awesome text, because, well, why not?"
            onChange=onChangeText
          />
        </label>
        <button>
          (str("Create"))
        </button>
      </form>
    </div>
  },
};
