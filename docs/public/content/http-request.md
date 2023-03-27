## Options

* target &lt;string&gt; specifies the target of the http request.
* method &lt;string&gt; the http method to use. GET/POST/PUT/DELETE
* body &lt;string, optional&gt; the body of a POST/PUT request.
* responseSelector &lt;string, optional&gt; a selector query of which to replace the innerHTML with the response of the request. Uses document.querySelectorAll internally.

## Methods

* `:trigger` triggers the http request.

## Example

```
{
	"$exampleEvent": {
		"type": "httpRequest",
		"target": "https://example.com/",
		"method": "POST",
		"body": {
			"id": 1,
			"name": "Lorem ipsum"
		},
		"responseSelector": "#result"
	}
}

```