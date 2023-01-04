# Game Project Workspace

## Operation

- Create workspace
- Import workspace
- Delete workspace

## Create Workspace

### Input Parameter

- Location : The directory where to create the worksapce folder
- Worksapce Name : The name of the new workspace folder
  - Cannot be a existing folder name
- Company Name : The default game company name
- Project Lead : The default lead developer name
- Project Namespace : The default namespace used for generated classes

### Creation Output

- New workspace folder
  - Contains *.workspace* document
  - Contains empty *Project* folder
- Update the Editor list of workspaces


## Import Workspace

### Input Parameter

- Workspace Directory : The folder of the workspace to import

### Import Output

- Update the Editor list of workspaces

## Delete Workspace

- The Editor is not able to delete a workspace by itself
- In order to delete a workspace you can
  - Delete the workspace folder
  - Rename the workspace folder
- At start up, if the Editor cannot find a worksapce previously saved in its workspace list, the workspace will be removed from the list and consider deleted

