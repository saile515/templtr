# templtr
## A lightweight framework for statically generating dynamic html pages, written in C++.

*Caution: Project is still very early in development, so expect many new features and breaking api changes.*

**templtr is:**
* Lightweight
* Easy to use
* Powerful
* Very fast

### Installation:
*Installation instructions coming soon.*

### Usage:
* Run `templtr init` to create a templtr project.
* Create template files in `/pages`, template files are like normal html files, but with curly brackets surrounding dynamic content.
* Create a directory in `/content` with the same name as the template (without .html).
* Fill the directory with a `.json` file for each page. The filename will be the slug when built.
* The JSON files should have all dynamic attributes within them.
* Run `templtr build <outdir>` to build the html files.

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
		{title}
	</body>
</html>
```

`content/test-page/page_1.json`
```json
{
	"title": "Hello World"
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
		Hello World
	</body>
</html>
```
