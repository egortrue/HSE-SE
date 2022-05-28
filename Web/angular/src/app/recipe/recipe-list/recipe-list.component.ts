import { Component } from '@angular/core';
import { Recipe } from '../recipe.interface';
import { RECIPES } from '../recipe.data';

@Component({
  selector: 'app-recipe-list',
  templateUrl: './recipe-list.component.html',
  styleUrls: ['./recipe-list.component.css'],
})
export class RecipeListComponent {
  recipes: Recipe[] = RECIPES;
}
