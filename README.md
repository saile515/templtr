# templtr
[![CMake](https://github.com/saile515/templtr/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/saile515/templtr/actions/workflows/cmake.yml)
**templtr (pronounced templator) is a lightweight framework for statically generating dynamic html pages, written in C++.**

*Caution: Project is still very early in development, so expect many new features and breaking api changes.*

## Features:
* Lightweight
* Easy to use
* Powerful
* Fast

## Installation:
### Option 1: Use prebuilt binaries
* Download and extract the latest release.
* Add to path. (optional)
### Option 2: Build from source
* Download and extract source from the latest release.
* Run `/usr/local/bin/cmake --build /path/to/source/build --config Release --target all --`
* Add to path. (optional)

## Usage:
* Run `templtr init` to create a templtr project.
* Create template files in `/pages`, template files are like normal html files, but with curly brackets surrounding dynamic content.
* Create a directory in `/content` with the same name as the template (without .html).
* Fill the directory with a `.json` file for each page. The filename will be the slug when built.
* Run `templtr build <outdir>` to build the html files.

## Example:
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
