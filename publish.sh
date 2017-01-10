#!/bin/sh
bundle exec rake docs:html && sh -c 'cd html && git add -A && git commit -m 'update' && git push origin' 
