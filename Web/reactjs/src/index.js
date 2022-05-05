import * as React from "https://cdn.skypack.dev/react@17.0.1";
import * as ReactDOM from "https://cdn.skypack.dev/react-dom@17.0.1";
import './index.css'


class App extends React.Component {
    render() {
        return <div>
            <h1 id="main-title">Untitled Task Manager</h1>
            <Table />
        </div>
    }
}


class Table extends React.Component {
    render() {
        return <div className='table'>
            <List title="To Do" />
            <List title="In progress" />
            <List title="Done" />
        </div>
    }
}


class List extends React.Component {
    constructor(props) {
        super(props);
        this.title = props.title;
        this.state = { tasks: [] };
    }

    addTask(task) {
        this.setState({ tasks: [task, ...this.state.tasks] });
    }

    removeTask(task) {
        this.setState({ tasks: this.state.tasks.filter(e => e.key.toString() !== task.id.toString()) })
    }

    render() {
        return <div className='list'>
            <p className="list-title">{this.title}</p>
            <InputTask list={this}/>
            {this.state.tasks}
        </div>
    }
}

// Базовая шаблонная "таска" - пораждает другие таски в списке
class InputTask extends React.Component {
    constructor(props) {
        super(props);
        this.list = props.list;
        this.state = { counter: 0, text: '' };
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
    }

    handleSubmit(event) {
        if (this.state.text !== '') {
            this.list.addTask(<ActiveTask key={this.state.counter} id={this.state.counter} list={this.list} text={this.state.text}/>);
            this.setState({ counter: this.state.counter + 1, text: '' });
        }
        event.preventDefault();
    }

    handleChange(event) {
        this.setState({ text: event.target.value });
    }

    render() {
        return <div className="task task-input">
            <form onSubmit={this.handleSubmit}>
                <input className='task-text task-input-text'
                    placeholder="Add new task"
                    value={this.state.text}
                    onChange={this.handleChange} />
            </form>
        </div>
    }
}


class ActiveTask extends React.Component {
    constructor(props) {
        super(props);
        this.id = props.id;
        this.list = props.list;
        this.text = props.text;
        this.handleRemove = this.handleRemove.bind(this);
    }

    handleRemove() {
        this.list.removeTask(this);
    }

    render() {
        return <div className="task task-active">
            <div className='task-text task-active-text'>{this.text}</div>
            <div onClick={this.handleRemove}><Button type="cancel"/></div>
        </div>
    }
}


class Button extends React.Component {
    constructor(props) {
        super(props);
        this.icon = props.type;
        this.class = "material-icons button button-" + props.type;
    }

    render() {
        return <button className={this.class}>{this.icon}</button>
    }
}


ReactDOM.render(
    <App />,
    document.getElementById('root')
);
