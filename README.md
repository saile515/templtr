# templtr
**templtr (pronounced templator) is a lightweight framework for statically generating dynamic html pages, written in C++.**

*Caution: Project is still very early in development, so expect many new features and breaking api changes.*

### Features:
* Lightweight
* Easy to use
* Powerful
* Fast

### Installation:
*Installation instructions coming soon.*

### Usage:
#### Creating a templtr project:
Run `templtr init` in the project root to create a templtr project.

#### `/pages` directory
`/pages` is where all html templates are stored. Template files are similar to `.html` files, including the file extension. They do however slightly differ in the file contents.

Pages what will only have a single instance should be named for example `page.html`. Pages with multiple routes should named for example `[page].html` (the square brackets tells templtr that the page is dynamic).

To include dynamic content in your template, surround a property name with curly brackets.
Example:
`{title}` will be replaced by the `title` property in the respective json file.

To include a list of dynamic content, surround an html node with square brackets, and include the property name somewhere within the node.
Example:
`[<li>{list}</li>]` will create an entry for every item within the `list` property.

To include an object, simply write the path to the object, like you would in JavaScript and other languages' objects.
Example:
`{object.name}` will be replaced by `name` sub-property of the `object` property.

To include a component, write the name of the component within a closed html start-tag.
Example:
`<navbar />` will be replaced by the content of the `navbar` component.

These features can be combined. You can have objects within an array in order to create lists with multiple dynamic properties.

#### `/content` directory
`/content` is where all dynamic content is stored. The content directory is also in charge of routing.

Single pages should be a `.json` file in the root of the `/content` directory with the same name as the template file (with a `.json` extension instead of `.html`).

Dynamic pages should be a directory in the root of the `/content` directory with the same name as the template file (without the `.html` extension). This directory should then be populated with a `.json` file for every entry of the dynamic page. The filename will be the slug of the built page (without `.json`).

*Note: Support for nested routing is currently not implemented, but is a planned feature.*

The `.json` files should be populated with the dynamic properties in the template files.

#### `/components` directory
`/components` is where all component files are stored. Components are reusable html snippets that can be used in multiple templates.

The filename will be the component name (without `.html`).

Components can include dynamic properties, which will be filled on a per-page basis in build-time.
*Note: This will only work well if all pages share the required dynamic properties.*

*Note: Support for sorting components in directories is currently not supported, but is a planned feature.*

### Example:
`pages/test-page.html`
```html
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta http-equiv="X-UA-Compatible" content="IE=edge" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Document</title>
	</head>
	<body>
		<h1>{title}</h1>
		<ul>
			[<li>{list}</li>]
		</ul>
		<article>
			<h2>{article.title}</h2>
			<p>{article.body}</p>
		</article>
	</body>
</html>
```

`content/test-page/page_1.json`
```json
{
	"title": "Hello World",
	"list": [
		"List item 1",
		"List item 2"
	],
	"article": {
		"title": "This is an article",
		"body": "Lorem ipsum dolor sit amet."
	}
}
```

After build:

`/out/test-page/page_1/index.html`
```html
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta http-equiv="X-UA-Compatible" content="IE=edge" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Document</title>
	</head>
	<body>
		<h1>Hello World</h1>
		<ul>
			<li>List item 1</li>
			<li>List item 2</li>
		</ul>
		<article>
			<h2>This is an article</h2>
			<p>Lorem ipsum dolor sit amet.</p>
		</article>
	</body>
</html>
```
