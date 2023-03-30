`/pages` is where all html templates are stored. Template files are similar to `.html` files, including the file extension. They do however slightly differ in the file contents.

Pages that will only have a single instance should be named for example `page.html`. Pages with multiple routes should named for example `[page].html` (the square brackets tells templtr that the page is dynamic).

To include dynamic content in your template, surround a property name with curly brackets.<br>
Example: `{title}` will be replaced by the title property in their respective json file.

To include a list of dynamic content, surround an html node with square brackets, and include the property name somewhere within the node.<br>
Example: `[<li>{list}</li>]` will create an entry for every item within the list property.

To include an object, simply write the path to the object, like you would in JavaScript and other languages' objects.<br>
Example: `{object.name}` will be replaced by the name sub-property of the object root-property.

To include a component, write the name of the component within a closed html start-tag.<br>
Example: `<navbar />` will be replaced by the content of the navbar component.

These features can be combined. You can have objects within an array in order to create lists with multiple dynamic properties.