<?php

/**
 * @file
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 */
?>
<?php if (!empty($title)) : ?>
  <h2><?php print $title; ?></h2>
<?php endif; ?>
<div class="<?php print $class; ?>"<?php print $attributes; ?>>
  <?php foreach ($rows as $row_number => $columns): ?>
    <?php foreach ($columns as $column_number => $item): ?>
      <?php if (!empty($item)):?>
        <div class="grid views-col tb-wall-grid <?php print $tb_wall_retro_classes[$row_number * $options['columns'] + $column_number];?> <?php print $column_classes[$row_number][$column_number]; ?>">
          <div class="grid-inner col-inner clearfix">
            <?php print $item; ?>
          </div>
        </div>
      <?php endif; ?>
    <?php endforeach; ?>
  <?php endforeach; ?>
</div>
