#!/usr/bin/env ruby
require 'json'
require 'yaml'

input_filename = ARGV[0]
output_filename = ARGV[1]

if input_filename == nil || output_filename == nil
    puts "yaml_to_json.rb input_file_name output_file_name"
    exit(-1)
end

input_yml = "{}"
File.open(input_filename) do |input_file|
  input_yml = input_file.read
end

output_json = JSON.dump(YAML::load(input_yml))

File.open(output_filename, 'w+') do |output_file|
  output_file.write(output_json)
end
