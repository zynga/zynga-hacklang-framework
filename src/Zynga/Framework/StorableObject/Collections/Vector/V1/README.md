# What is this?
----
A importer/exporter vector collection for storable objects that is type safe.

# Motivation
----
We previously did not have a way of importing a storable/box object given a
collection. With this change, we introduce a type safe way of importing
a type using dynamic class creation along with type hints to make allow
us to import a list of storable/box objects.
See: https://docs.hhvm.com/hack/generics/erasure

Check out the class docs for more info.

# How do you use this?
----
Check out the unit tests for usage.

# Why was the previous collection not just updated?
---
The previous collection made no assumptions for the refined type a
collection wrapped. Because of this, you could stand up a storable
vector of non storable objects and would not see an error when an import
was attempted. In addition, type erasure for the language made it
impossible for us to stand up the expected type when we tried to
deserialize a payload.

Given these issues, a new version of the collection was created. The old
version of the collection has since been deprecated for this version. Creating
this new version also prevents breakage of the older class use cases.
